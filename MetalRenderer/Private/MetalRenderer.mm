#include <MetalRenderer/MetalRenderer.h>

#include "MetalSwapChain.h"

#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalCommandBuffer.h>
#include <MetalCpp/MetalRenderPass.h>
#include <MetalCpp/MetalCommandEncoder.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalVertexDescriptor.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalTypes.h>

#include <Renderer/Effect.h>
#include <Renderer/Buffer.h>
#include <Renderer/Object3d.h>

#include "RendererToMetalConverter.h"
#include "DeviceResourceIdImpl.h"

#include <iostream>
#include <utility>

#include <Renderer/Command.h>
#include <Renderer/Camera.h>

#import <Metal/Metal.h>

namespace Summit
{
    class CommandExecutor : public ICommandVisitor
    {
    public:
        CommandExecutor(MTL::RenderCommandEncoder& encoder)
            : mEncoder(encoder)
        {}
        
        void Visit(const BeginCommand& cmd) override
        {
            
        }
        
        void Visit(const SetViewportCommand& cmd) override
        {
            MTL::Viewport mtlViewport;
            mtlViewport.originX = cmd.viewport.rect.x;
            mtlViewport.originY = cmd.viewport.rect.y;
            mtlViewport.width = cmd.viewport.rect.width;
            mtlViewport.height = cmd.viewport.rect.height;
            mtlViewport.znear = cmd.viewport.near;
            mtlViewport.zfar = cmd.viewport.far;
            
            mEncoder.SetViewport(mtlViewport);
        }
        
        void Visit(const SetScissorRectCommand& cmd) override
        {
            MTL::ScissorRect mtlScissorRect;
            mtlScissorRect.x = cmd.rect.x;
            mtlScissorRect.y = cmd.rect.y;
            mtlScissorRect.width = cmd.rect.width;
            mtlScissorRect.height = cmd.rect.height;
            
            mEncoder.SetScissorRect(mtlScissorRect);
        }
        
        void Visit(const SetVertexBufferOffsetCommand& cmd) override
        {
            mEncoder.SetVertexBufferOffset(cmd.offset, cmd.index);
        }
        
        void Visit(const DrawIndexedPrimitivesCommand& cmd) override
        {
            mEncoder.DrawIndexedPrimitives(MTL::PrimitiveType::Triangle,
                                           cmd.count,
                                           MTL::IndexType::UInt32,
                                           *indexBuffer,
                                           cmd.offset);
        }
        
        void Visit(const SetVertexBytesCommand& cmd) override
        {
            mEncoder.SetVertexBytes(cmd.data.data(), cmd.data.size(), cmd.bufIdx);
        }
        
    public:
        MTL::Buffer* indexBuffer{ nullptr };
        
        
    private:
        MTL::RenderCommandEncoder& mEncoder;
    };
}

namespace Memory
{
    template<uint64_t Alignment, typename T>
    constexpr T Align(T Val)
    {
        static_assert(std::is_integral_v<T> || std::is_pointer_v<T>, "Align expects an integer or pointer type");
        return (T)(((uint64)Val + Alignment - 1) & ~(Alignment - 1));
    }

    template<uint64_t Alignment, typename T>
    constexpr T AlignDown(T Val)
    {
        static_assert(std::is_integral_v<T> || std::is_pointer_v<T>, "AlignDown expects an integer or pointer type");
        return (T)(((uint64)Val) & ~(Alignment - 1));
    }
}

namespace
{
    MTL::VertexFormat ConvertToMTLVertexFormat(Summit::Format format)
    {
        if(format == Summit::Format::Float2) return MTL::VertexFormat::Float2;
        if(format == Summit::Format::Float3) return MTL::VertexFormat::Float3;
        if(format == Summit::Format::Float4) return MTL::VertexFormat::Float4;
        if(format == Summit::Format::UChar4) return MTL::VertexFormat::UChar4;
        
        return MTL::VertexFormat::Float4;
    }
    
    uint32_t GetSizeFromFormat(Summit::Format format)
    {
        if(format == Summit::Format::Float2) return 2 * sizeof(float);
        if(format == Summit::Format::Float3) return 3 * sizeof(float);
        if(format == Summit::Format::Float4) return 4 * sizeof(float);
        if(format == Summit::Format::UChar4) return 4 * sizeof(char);
        
        return 0;
    }
    
    template<typename U, typename T>
    void ReleaseObject(T& object)
    {
        @autoreleasepool
        {
            U native = (__bridge U)object.GetPtr();
            [native release];
            native = nil;
        }
    }
    
    template<typename U, typename T>
    void ReleaseResourceObject(T& object)
    {
        object.SetPurgeableState(MTL::PurgeableState::Empty);
        ReleaseObject<U>(object);
    }
    
    void LogDeviceInfo(const MTL::MetalDevice& device)
    {
        std::cout << "Device name: " << device.GetDeviceName() << std::endl;
        
        std::cout << "Is low power: ";
        if(MTL::Device::IsLowPower(device))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
            
        std::cout << "Is headles: ";
        if(MTL::Device::IsHeadless(device))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
    }
}

class MetalRenderAPI;

namespace Summit
{
    MetalRenderAPI::MetalRenderAPI()
    {
        @autoreleasepool
        {
            auto metalDevices = MTL::Device::CopyAllDevices();
            
            std::cout << "---------- " << metalDevices.GetSize() << " Devices supporting Metal API: -----------" << std::endl;
            for(size_t deviceIdx{ 0 }; deviceIdx < metalDevices.GetSize(); ++deviceIdx)
            {
                const auto device = metalDevices[deviceIdx];
                LogDeviceInfo(device);
            }
        }

        mDevice = std::make_shared<MTL::MetalDevice>(MTL::CreateSystemDefaultDevice());
        mCommandQueue = mDevice->NewCommandQueue();
        mTransferQueue = mDevice->NewCommandQueue();
        
        if(mDevice)
        {
            mDefaultLibrary = std::make_unique<MTL::Library>(mDevice->CreateNewDefaultLibrary());
            const auto funcNames = mDefaultLibrary->GetFunctionNames();
            
            std::cout << "Default library functions: \n";
            
            for(const auto& func : funcNames)
            {
                std::cout << func << '\n';
            }
            
            MTL::DepthStencilDescriptor depthDesc;
            depthDesc.SetDepthCompareFunction(MTL::CompareFunction::LessEqual);
            depthDesc.SetDepthWriteEnabled(true);
            
            mDepthStencilStates.push_back(mDevice->NewDepthStencilState(depthDesc));
            
            MTL::DepthStencilDescriptor noWriteDepthDesc;
            noWriteDepthDesc.SetDepthCompareFunction(MTL::CompareFunction::Less);
            noWriteDepthDesc.SetDepthWriteEnabled(false);
            
            mDepthStencilStates.push_back(mDevice->NewDepthStencilState(noWriteDepthDesc));
        }

        mFrameCmdBufferSemaphore = dispatch_semaphore_create(1);

        mFragUniBufferId = CreateBuffer(CreateUniformBufferDesc<FragmentBufferSharedData>(nullptr));
        mVertUniBufferId = CreateBuffer(CreateUniformBufferDesc<VertexBufferSharedData>(nullptr));
    }
    
    MetalRenderAPI::~MetalRenderAPI()
    {}
    
    std::unique_ptr<Renderer::ISwapChain> MetalRenderAPI::CreateSwapChain(const uint32_t width, const uint32_t height)
    {
        TextureId depthAttachment = CreateTexture2D(width, height);
        return std::make_unique<SwapChain>(width, height, *mDevice, mCommandQueue, depthAttachment, this);
    }
    
    void* MetalRenderAPI::GetNativeHandle()
    {
        return mDevice->mNativeHandle;
    }

    GraphicsAPI MetalRenderAPI::GetAPI() const
    {
        return GraphicsAPI::Metal;
    }
    
    std::string MetalRenderAPI::GetDeviceDescription() const
    {
        if(!mDevice)
            throw std::runtime_error("Metal device is not created!");
        
        return mDevice->GetDeviceName();
    }

    BufferId MetalRenderAPI::CreateBuffer(const BufferDesc& desc)
    {
        //ASSERT(desc.size > 0);
        
        MTL::StorageMode storageMode = to_metal_type(desc.memoryType);
        
        BufferId resultId;
        
        @autoreleasepool
        {
            if(storageMode == MTL::StorageMode::Private)
            {                
                //ASSERT(desc.data);
                auto cpuBuffer = mDevice->NewBuffer(desc.data, desc.size, MTL::ResourceOptions::StorageModeShared);
                auto gpuBuffer = mDevice->NewBuffer(desc.size, MTL::ResourceOptions::StorageModePrivate);
                
                auto cmdBuffer = mTransferQueue.CommandBufferWithUnretainedReferences();

                auto blitEncoder = cmdBuffer.BlitCommandEncoder();
                blitEncoder.CopyFromBuffer(cpuBuffer, 0, gpuBuffer, 0, desc.size);
                blitEncoder.EndEncoding();
                
                auto ctx = std::make_shared<MTL::Buffer>(std::move(cpuBuffer));
                
                // Release CPU buffer when command buffer finishes it's work
                cmdBuffer.AddCompletedHandler([buffer = std::move(ctx)](const MTL::CommandBuffer& cmdBuf){
                    ReleaseObject<id<MTLBuffer>>(*buffer);
                });
                
                cmdBuffer.Commit();
                
                mBuffers.push_back(std::move(gpuBuffer));
                resultId = BufferId{ static_cast<uint32_t>(mBuffers.size() - 1) };
            }
            else
            {
                MTL::ResourceOptions options;
                
                if(1)   // IS_MAC?
                {
                    options = MTL::ResourceOptions::StorageModeShared;
                }
                
                
                auto buffer = [&desc, &options, device = mDevice](){
                    if(desc.data)
                        return device->NewBuffer(desc.data, desc.size, options);
                    else
                        return device->NewBuffer(desc.size, options);
                }();
                
                if(mBufferFreeList.empty())
                {
                    mBuffers.push_back(std::move(buffer));
                    resultId = BufferId{ static_cast<uint32_t>(mBuffers.size() - 1) };
                }
                else
                {
                    auto firstFreeSlot = mBufferFreeList.front();
                    mBufferFreeList.pop_front();
                    
                    mBuffers[firstFreeSlot] = std::move(buffer);
                    resultId = BufferId{ firstFreeSlot };
                }
            }
        }
        
        return resultId;
    }
    
    PipelineId MetalRenderAPI::CreatePipeline(const PipelineDescriptor& desc)
    {
        std::vector<ResourceId> resourceDependencies;
        
        PipelineId deviceId{ static_cast<uint32_t>(mRenderPipelines.size()) };
        
        @autoreleasepool
        {
            if(!mDefaultLibrary)
            {
                throw std::runtime_error("default library not created");
            }
            
            MTL::RenderPipelineDescriptor descriptor;
            descriptor.SetLabel(desc.GetPipelineName());
            
            const auto defaultLibraryFunctions = mDefaultLibrary->GetFunctionNames();
            const auto& moduleDescriptors = desc.GetModuleDescriptors();
            
            // Setup pipeline functions
            for(const auto& moduleDescriptor : moduleDescriptors)
            {
                auto stageResources = ProcessStage(deviceId, moduleDescriptor, defaultLibraryFunctions, descriptor);
                std::move(std::begin(stageResources), std::end(stageResources), std::back_inserter(resourceDependencies));
            }
            
            // Setup attribute descriptors & layout
            MTL::VertexDescriptor vertexDescriptor;
            auto attributes = vertexDescriptor.GetAttributes();
            auto layouts = vertexDescriptor.GetLayouts();
            
            const auto bindings = desc.GetAttributeBindingCount();
            for(uint32_t bindingId{ 0 }; bindingId < bindings; ++bindingId)
            {
                uint32_t layoutStride{ 0 };
                
                const auto& attributeDescs = desc.GetAttributes(bindingId);
                for(uint32_t attributeId{ 0 }; attributeId < attributeDescs.size(); ++attributeId)
                {
                    const auto& attributeDescriptor = attributeDescs[attributeId];
                    attributes[bindingId] = MTL::VertexAttributeDescriptor(ConvertToMTLVertexFormat(attributeDescriptor), bindingId, layoutStride);
                    layoutStride += GetSizeFromFormat(attributeDescriptor);
                }
                
                layouts[bindingId] = MTL::VertexBufferLayoutDescriptor(Memory::Align<4>(layoutStride));
            }
            
            // TODO: Add settings to effect
            descriptor.SetDepthAttachmentPixelFormat(MTL::PixelFormat::Depth32Float);

            if(desc.enableBlending)
            {
                // Setup pipeline
                descriptor.SetVertexDescriptor(vertexDescriptor);

                auto colorAttachments = descriptor.GetAttachments();
                colorAttachments[0].EnableBlending(true);
                colorAttachments[0].SetRGBBlendOperation(MTL::BlendOperation::Add);
                colorAttachments[0].SetAlphaBlendOperation(MTL::BlendOperation::Add);
                colorAttachments[0].SetSourceRGBBlendFactor(MTL::BlendFactor::SourceAlpha);
                colorAttachments[0].SetSourceAlphaBlendFactor(MTL::BlendFactor::SourceAlpha);
                colorAttachments[0].SetDestinationRGBBlendFactor(MTL::BlendFactor::OneMinusSourceAlpha);
                colorAttachments[0].SetDestinationAlphaBlendFactor(MTL::BlendFactor::OneMinusSourceAlpha);
            }
            
            auto renderPipelineState = mDevice->NewRenderPipelineState(descriptor);
            
            mRenderPipelines.push_back(std::move(renderPipelineState));
        }
        
        mPipelineStateCache[deviceId].resources = std::move(resourceDependencies);
        
        return deviceId;
    }
    
    TextureId MetalRenderAPI::CreateTexture2D(const uint32_t width, const uint32_t height, const void* data)
    {
        MTL::TextureDescriptor texDesc(width, height);
        texDesc.SetMipMapLevelCount(8);
        
        auto texture = mDevice->NewTexture(texDesc);
        
        if(data)
        {
            MTL::Region region;
            region.origin.x = 0;
            region.origin.y = 0;
            region.size.width = width;
            region.size.height = height;
            
            texture.ReplaceRegion(region, 0, data, width * 4);
            
            auto cmdBuffer = mTransferQueue.CommandBufferWithUnretainedReferences();
            auto cmdEncoder = cmdBuffer.BlitCommandEncoder();
            cmdEncoder.GenerateMipmaps(texture);
            cmdEncoder.EndEncoding();
            
            cmdBuffer.Commit();
        }
        
        mTextures.push_back(std::move(texture));
        
        return { static_cast<uint32_t>(mTextures.size() - 1) };
    }
    
    TextureId MetalRenderAPI::CreateTexture2D(const uint32_t width, const uint32_t height)
    {
        MTL::TextureDescriptor texDesc(width, height);
        texDesc.SetPixelFormat(MTL::PixelFormat::Depth32Float);
        texDesc.SetUsage(MTL::TextureUsage::RenderTarget);
        texDesc.SetStorageMode(MTL::StorageMode::Private);
        
        auto texture = mDevice->NewTexture(texDesc);
        mTextures.push_back(std::move(texture));
        
        return { static_cast<uint32_t>(mTextures.size() - 1) };
    }

    TextureId MetalRenderAPI::CreateTexture2D(const TextureDescriptor& desc)
    {
        MTL::TextureDescriptor texDesc(desc.width, desc.height);
        texDesc.SetPixelFormat(MTL::PixelFormat::Depth32Float);
        texDesc.SetUsage(MTL::TextureUsage::RenderTarget);
        texDesc.SetStorageMode(MTL::StorageMode::Private);

        auto texture = mDevice->NewTexture(texDesc);
        texture.SetLabel("Texture");
        
        mTextures.push_back(std::move(texture));

        return { static_cast<uint32_t>(mTextures.size() - 1) };
    }
    
    SamplerId MetalRenderAPI::CreateSampler(const SamplerDesc& desc)
    {
        MTL::SamplerDescriptor samplerDesc(MTL::SamplerMinMagFilter::Linear, MTL::SamplerMinMagFilter::Linear);
        auto samplerState = mDevice->NewSamplerState(samplerDesc);
        mSamplers.push_back(std::move(samplerState));
        
        return{ static_cast<uint32_t>(mSamplers.size() - 1) };
    }
    
    void MetalRenderAPI::UpdateTexture2D(const uint32_t width, const uint32_t height, const void* data)
    {
        MTL::Region region;
        region.origin.x = 0;
        region.origin.y = 0;
        region.size.width = width;
        region.size.height = height;
    }
    
    void MetalRenderAPI::UpdateBuffer(BufferId bufferId, void* data, const uint32_t size)
    {
        auto& buffer = mBuffers[bufferId.id];
        
        if(buffer.GetStorageMode() == MTL::StorageMode::Shared)
        {
            auto* bufferContents = buffer.Contents();
            memcpy(bufferContents, data, size);
        }
        else
        {
            throw std::runtime_error("Unimplemented!");
        }
    }
    
    void MetalRenderAPI::RenderObject3d(const Renderer::Object3d& object, const PipelineId& effect, const CmdList* cmdList)
    {
        if(!object.GetVertexBufferPointer())
            return;
        
        const auto& vertexBuffer = *object.GetVertexBufferPointer();
        const auto dataStreamDescriptors = vertexBuffer.GetDataDescriptors();
        const auto indexDataDescriptor = vertexBuffer.GetIndicesDescriptor();
        if(indexDataDescriptor.count == 0)
            return;
        
        auto& renderEncoder = mRenderPass.GetRenderCommandEncoder();
        renderEncoder.SetRenderPipelineState(mRenderPipelines[effect.id]);
        
        uint32_t bufferBindingIndex{ 0 };
        for(const auto& attributeDescriptor : dataStreamDescriptors)
        {
            const auto index = &attributeDescriptor  - &dataStreamDescriptors[0];
            renderEncoder.SetVertexBuffer(mBuffers[dataStreamDescriptors[index].resourceId.id], 0, bufferBindingIndex++);
        }
        
        if(auto result = mPipelineStateCache.find(effect); result != mPipelineStateCache.end())
        {
            auto [pipelineId, pipelineState] = *result;
            
            // Bind argument buffers
            for(const auto& argBuffer : pipelineState.argumentBuffers)
            {
                if(argBuffer.stage == ModuleStage::Vertex)
                {
                    renderEncoder.SetVertexBuffer(mBuffers[argBuffer.buffer.id], 0, bufferBindingIndex++);
                }
                else if(argBuffer.stage == ModuleStage::Fragment)
                {
                    renderEncoder.SetFragmentBuffer(mBuffers[argBuffer.buffer.id], 0, 0);
                }
                else
                {
                    //ASSERT(0 && "Used resource other than buffer or texture");
                }
            }
            
            // Map resources from argument buffers to GPU address space
            for(const auto& resourceId : pipelineState.resources)
            {
                if(resourceId.type == UniformType::Buffer)
                {
                    renderEncoder.UseResource(mBuffers[resourceId.id].GetPtr(), MTL::ResourceUsage::Read);
                }
                else if(resourceId.type == UniformType::Texture)
                {
                    renderEncoder.UseResource(mTextures[resourceId.id].GetPtr(), MTL::ResourceUsage::Sample);
                }
                else
                {
                    //ASSERT(0 && "Used resource other than buffer or texture");
                }
            }
        }
        
        // Bind uniforms buffers
        for(const auto& ubo : object.buffers)
        {
            if(ubo.shaderType == 0)
                renderEncoder.SetVertexBuffer(mBuffers[ubo.buffer.id], 0, ubo.index);
            else
                renderEncoder.SetFragmentBuffer(mBuffers[ubo.buffer.id], 0, ubo.index);
        }
        
        // Bind textures
        for(const auto& texture : object.textures)
        {
            if(texture.shaderType == 0)
            {}
            else
                renderEncoder.SetFragmentTexture(mTextures[texture.texture.id], texture.index);
        }
        
        
        if(mRenderPassIndex == 1)
        {
            // Disable depth write
            renderEncoder.SetDepthStencilState(mDepthStencilStates.back());
        }
        else
        {

            // Set depth state
            renderEncoder.SetDepthStencilState(mDepthStencilStates.front());
            renderEncoder.SetVertexBuffer(mBuffers[mVertUniBufferId.id], 0, 3);
            renderEncoder.SetFragmentBuffer(mBuffers[mFragUniBufferId.id], 0, 1);
        }

        if(cmdList)
        {
            Summit::CommandExecutor executor(renderEncoder);
            executor.indexBuffer = &mBuffers[indexDataDescriptor.resourceId.id];
            
            for(const auto& cmd : cmdList->mCommands)
            {
                cmd.Execute(executor);
            }
        }
        else
        {
            renderEncoder.DrawIndexedPrimitives(MTL::PrimitiveType::Triangle,
                                                indexDataDescriptor.count,
                                                MTL::IndexType::UInt32,
                                                mBuffers[indexDataDescriptor.resourceId.id],
                                                0);
        }
    }
    
    void MetalRenderAPI::RenderObject3dInstanced(const Renderer::Object3d& object, const PipelineId& effect, uint32_t instanceCount)
    {
        if(!object.GetVertexBufferPointer())
            return;
        
        auto& renderEncoder = mRenderPass.GetRenderCommandEncoder();
        renderEncoder.SetRenderPipelineState(mRenderPipelines[effect.id]);
        
        const auto& vertexBuffer = *object.GetVertexBufferPointer();
        
        // Bind vertex attributes
        const auto dataStreamDescriptors = vertexBuffer.GetDataDescriptors();
        const auto indexDataDescriptor = vertexBuffer.GetIndicesDescriptor();
        
        uint32_t bufferBindingIndex{ 0 };
        for(const auto& attributeDescriptor : dataStreamDescriptors)
        {
            const auto index = &attributeDescriptor  - &dataStreamDescriptors[0];
            renderEncoder.SetVertexBuffer(mBuffers[dataStreamDescriptors[index].resourceId.id], 0, bufferBindingIndex++);
        }
        
        if(auto result = mPipelineStateCache.find(effect); result != mPipelineStateCache.end())
        {
            auto [pipelineId, pipelineState] = *result;
            
            // Bind argument buffers
            for(const auto& argBuffer : pipelineState.argumentBuffers)
            {
                if(argBuffer.stage == ModuleStage::Vertex)
                {
                    renderEncoder.SetVertexBuffer(mBuffers[argBuffer.buffer.id], 0, bufferBindingIndex++);
                }
                else if(argBuffer.stage == ModuleStage::Fragment)
                {
                    renderEncoder.SetFragmentBuffer(mBuffers[argBuffer.buffer.id], 0, 0);
                }
                else
                {
                    //ASSERT(0 && "Used resource other than buffer or texture");
                }
            }
            
            // Map resources from argument buffers to GPU address space
            for(const auto& resourceId : pipelineState.resources)
            {
                if(resourceId.type == UniformType::Buffer)
                {
                    renderEncoder.UseResource(mBuffers[resourceId.id].GetPtr(), MTL::ResourceUsage::Read);
                }
                else if(resourceId.type == UniformType::Texture)
                {
                    renderEncoder.UseResource(mTextures[resourceId.id].GetPtr(), MTL::ResourceUsage::Sample);
                }
                else
                {
                    //ASSERT(0 && "Used resource other than buffer or texture");
                }
            }
        }
        
        // Bind uniforms buffers
        for(const auto& ubo : object.buffers)
        {
            if(ubo.shaderType == 0)
                renderEncoder.SetVertexBuffer(mBuffers[ubo.buffer.id], 0, ubo.index);
            //else
            //    renderEncoder.SetFragmentBuffer(mBuffers[ubo.buffer.id], 0, ubo.index);
        }
        
        if(mRenderPassIndex == 1)
        {
            // Disable depth write
            renderEncoder.SetDepthStencilState(mDepthStencilStates.back());
        }
        else
        {
            // Set depth state
            renderEncoder.SetDepthStencilState(mDepthStencilStates.front());
        }
        
        // Bind samplers
        //renderEncoder.SetFragmentSamplerState(mSamplers.front(), 0);
        //renderEncoder.SetCullMode(MTL::CullMode::Back);
        //renderEncoder.SetFrontFacingWinding(MTL::Winding::Clockwise);
        
        renderEncoder.DrawIndexedPrimitives(MTL::PrimitiveType::Triangle,
                                            indexDataDescriptor.count,
                                            MTL::IndexType::UInt32,
                                            mBuffers[indexDataDescriptor.resourceId.id],
                                            0, instanceCount);
    }
    
    void MetalRenderAPI::BeginFrame()
    {
        dispatch_semaphore_wait(mFrameCmdBufferSemaphore, DISPATCH_TIME_FOREVER);
    
        dispatch_semaphore_t cmdBufferSemCopy = mFrameCmdBufferSemaphore;
        dispatch_retain(cmdBufferSemCopy);
        
        @autoreleasepool {
            mFrameCmdBuffer = mCommandQueue.CreateCommandBuffer();
        }
            
        mFrameCmdBuffer.AddCompletedHandler([cmdBufferSemCopy](const MTL::CommandBuffer& cmdBuff) {
            dispatch_semaphore_signal(cmdBufferSemCopy);
            dispatch_release(cmdBufferSemCopy);
        });
    }
    
    void MetalRenderAPI::EndFrame()
    {
        mFrameCmdBuffer.Commit();
    }

    void MetalRenderAPI::BeginScene(const Camera& camera)
    {
        mSceneVertexData.viewMatrix = camera.GetViewMatrix();
        mSceneVertexData.projectionMatrix = camera.GetProjectionMatrix();
        mSceneVertexData.inverseViewMatrix = camera.GetViewMatrix().inverse();

        IRenderer::UpdateBuffer<VertexBufferSharedData>(mVertUniBufferId, mSceneVertexData);
        IRenderer::UpdateBuffer<FragmentBufferSharedData>(mFragUniBufferId, mSceneFragData);
    }

    void MetalRenderAPI::EndScene()
    {

    }
    
    void MetalRenderAPI::BeginRenderPass(Renderer::ISwapChain& swapChain, const char* name)
    {
        mRenderPassIndex = (mRenderPassIndex + 1) % 2;
        
        auto& metalSwapChain = static_cast<SwapChain&>(swapChain);
        const auto& drawable = metalSwapChain.GetDrawable();
        
        id<CAMetalDrawable> drw = ((__bridge id<CAMetalDrawable>)drawable.mNativeHandle);
        id<MTLTexture> texture = drw.texture;
        
        // TODO: create descriptor of swap chain details
        MTL::RenderPassDescriptor renderPassDesc;
        
        auto colorAttachments = renderPassDesc.GetColorAttachments();
        colorAttachments[0].SetLoadAction(mRenderPassIndex == 1 ? MTL::LoadAction::Load : MTL::LoadAction::Clear);
        //colorAttachments[0].SetLoadAction(MTL::LoadAction::Clear);
        colorAttachments[0].SetStoreAction(MTL::StoreAction::Store);
        colorAttachments[0].SetTexture(MTL::Texture(texture));
        colorAttachments[0].SetClearColor(MTL::ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f });
        
        auto depthAttachment = renderPassDesc.GetDepthAttachment();
        depthAttachment.SetClearDepth(1.0f);
        depthAttachment.SetLoadAction(mRenderPassIndex == 1 ? MTL::LoadAction::Load : MTL::LoadAction::Clear);
        //depthAttachment.SetLoadAction(MTL::LoadAction::Clear);
        depthAttachment.SetStoreAction(MTL::StoreAction::Store);
        depthAttachment.SetTexture(mTextures[metalSwapChain.GetDepthAttachmentId().id]);
        
        mRenderPass.Begin(renderPassDesc, mCommandQueue, name);
    }

    void MetalRenderAPI::BeginRenderPass(const RenderPassDescriptor& desc, const char* name)
    {
        MTL::RenderPassDescriptor renderPassDesc;
        auto depthAttachment = renderPassDesc.GetDepthAttachment();
        depthAttachment.SetClearDepth(1.0f);
        depthAttachment.SetLoadAction( MTL::LoadAction::Clear);
        depthAttachment.SetStoreAction(MTL::StoreAction::Store);
        depthAttachment.SetTexture(mTextures[desc.depthAttachment.id]);

        mRenderPass.Begin(renderPassDesc, mCommandQueue, name);
    }
    
    void MetalRenderAPI::EndRenderPass()
    {
        mRenderPass.End();
    }
    
    void MetalRenderAPI::CopyBuffer(BufferId srcBuffId, const uint32_t srcIndex, BufferId dstBuffId, const uint32_t dstIndex, const uint32_t size)
    {
        const auto& srcBuffer = mBuffers[srcBuffId.id];
        const auto& dstBuffer = mBuffers[dstBuffId.id];
        
        auto cmdBuffer = mTransferQueue.CommandBufferWithUnretainedReferences();
        
        auto blitEncoder = cmdBuffer.BlitCommandEncoder();
        blitEncoder.CopyFromBuffer(srcBuffer, 0, dstBuffer, 0, size);
        blitEncoder.EndEncoding();
        cmdBuffer.Commit();
        
        //TODO: HACK
        cmdBuffer.WaitUntilCompleted();
    }
    
    void MetalRenderAPI::ReleaseBuffer(BufferId bufferId)
    {
        ReleaseObject<id<MTLBuffer>>(mBuffers[bufferId.id]);
        mBufferFreeList.push_back(bufferId.id);
    }

    void MetalRenderAPI::ReleaseTexture(TextureId textureId)
    {
        ReleaseObject<id<MTLTexture>>(mTextures[textureId.id]);
    }

    void MetalRenderAPI::ReleaseSampler(SamplerId samplerId)
    {
        ReleaseObject<id<MTLSamplerState>>(mSamplers[samplerId.id]);
    }

    void MetalRenderAPI::ReleasePipeline(PipelineId pipelineId)
    {
        ReleaseObject<id<MTLRenderPipelineState>>(mRenderPipelines[pipelineId.id]);
    }
    
    std::vector<MetalRenderAPI::ResourceId> MetalRenderAPI::ProcessStage(PipelineId id, const ModuleDescriptor& module, const std::vector<std::string>& functions, MTL::RenderPipelineDescriptor& desc)
    {
        using namespace MTL;
        
        std::vector<MetalRenderAPI::ResourceId> resourceIds;
        
        @autoreleasepool
        {
            if(std::find(functions.begin(), functions.end(), module.moduleEntryPoint) == functions.end())
            {
                throw std::runtime_error("default library does not contain requested function");
            }
            
            // TODO: Cache these & reuse
            const auto function = mDefaultLibrary->CreateNewFunctionWithName(module.moduleEntryPoint);
            const auto functionType = function.GetFunctionType();
            //ASSERT(functionType == to_metal_type(module.type));
            
            // Set stage functions
            switch (module.type)
            {
                case ModuleStage::Undefined: (functionType == FunctionType::Vertex) ? desc.SetVertexFunction(function) : desc.SetFragmentFunction(function); break;
                case ModuleStage::Vertex: desc.SetVertexFunction(function); break;
                case ModuleStage::Fragment: desc.SetFragmentFunction(function); break;
            };
            
            auto EncodeBindingArguments = [this](ArgumentEncoder& argumentEncoder,
                                                       const auto& bindingDescriptor,
                                                       std::vector<MetalRenderAPI::ResourceId>& resourceIds) {
                const auto resourceIdsCount = resourceIds.size();
                
                for(const auto& uniform : bindingDescriptor)
                {
                    const auto& resourceIndex = &uniform - &bindingDescriptor[0];
                    
                    if(uniform.type == UniformType::Texture)
                    {
                        argumentEncoder.SetTexture(mTextures[uniform.id], resourceIndex);
                        resourceIds.push_back({ uniform.id, uniform.type });
                    }
                    else if(uniform.type == UniformType::Sampler)
                    {
                        argumentEncoder.SetSamplerState(mSamplers[uniform.id], resourceIndex); break;
                    }
                }
            };
            
            for(const auto& bindingDescriptor : module.bindingDescriptors)
            {
                if(bindingDescriptor.empty())
                    continue;
                
                // Create argument encoder for specific binding
                auto argumentEncoder = function.NewArgumentEncoder(&bindingDescriptor - &module.bindingDescriptors[0]);
                
                const auto argumentBufferLength = argumentEncoder.GetEncodedLength();
                auto argumentBuffer = mDevice->NewBuffer(argumentBufferLength, ResourceOptions::StorageModeShared);
                argumentBuffer.SetLabel("Argument buffer");
                argumentEncoder.SetArgumentBuffer(argumentBuffer, 0);
                
                EncodeBindingArguments(argumentEncoder, bindingDescriptor, resourceIds);
                mPipelineStateCache[id].argumentBuffers.push_back({ { static_cast<uint32_t>(mBuffers.size()) }, module.type });
                
                mBuffers.push_back(std::move(argumentBuffer));
            }
        }
        
        return resourceIds;
    }
}

extern "C"
{
    Summit::IRenderer* createRenderer()
    {
        return new Summit::MetalRenderAPI();
    }
}
