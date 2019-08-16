#pragma once

#include <Renderer/Renderer.h>
#include <Renderer/Effect.h>
#include <Renderer/DeviceResourceId.h>
#include <Renderer/Math/Math.h>

#include <MetalCpp/MetalCommandQueue.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalBuffer.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalSampler.h>
#include <MetalCpp/MetalDepthStencil.h>
#include <MetalCpp/MetalHeap.h>
#include <MetalCpp/MetalCommandBuffer.h>

#include "MetalRenderPass.h"

#include <memory> // For unique_ptr
#include <vector>
#include <deque>
#include <unordered_map>

#include <dispatch/dispatch.h>

namespace Summit
{
    class MetalDeviceWrapper;

    struct VertexBufferSharedData
    {
        Math::Float4x4 viewMatrix;
        Math::Float4x4 projectionMatrix;
        Math::Float4x4 inverseViewMatrix;
    };

    struct FragmentBufferSharedData
    {
        float brightness{ 1.0f };
        float reflectivity{ 1.0f };
        float shininess{ 10.0f };
        Math::Float3 light_color{ 1.0f, 1.0f, 1.0f };
    };
    
    class MetalRenderAPI : public IRenderer
    {
        struct ResourceId
        {
            uint32_t id{ 0 };
            UniformType type{ UniformType::Undefined };
        };
        
        struct ArgumentBufferDesc
        {
            BufferId buffer;
            ModuleStage stage;
        };
        
        struct PipelineState
        {
            std::vector<ResourceId> resources;
            std::vector<ArgumentBufferDesc> argumentBuffers;
        };
        
    public:
        MetalRenderAPI();
        virtual ~MetalRenderAPI();
        
        std::unique_ptr<Renderer::ISwapChain> CreateSwapChain(const uint32_t width, const uint32_t height) override;
        
        void* GetNativeHandle() override;
        
        GraphicsAPI GetAPI() const override;
        std::string GetDeviceDescription() const override;
        
        BufferId    CreateBuffer(const BufferDesc& desc) override;
        PipelineId  CreatePipeline(const PipelineDescriptor& desc) override;
        TextureId   CreateTexture2D(const uint32_t width, const uint32_t height, const void* data) override;
        TextureId   CreateTexture2D(const uint32_t width, const uint32_t height) override;
        TextureId   CreateTexture2D(const TextureDescriptor& desc) override;
        SamplerId   CreateSampler(const SamplerDesc& desc) override;
        
        
        void UpdateTexture2D(const uint32_t width, const uint32_t height, const void* data) override;
        void UpdateBuffer(BufferId bufferId, void* data, uint32_t size) override;
        
        void RenderObject3d(const Renderer::Object3d& object, const PipelineId& effect, const CmdList* cmdList) override;
        void RenderObject3dInstanced(const Renderer::Object3d& object, const PipelineId& effect, uint32_t instanceCount) override;
        
        void BeginFrame() override;
        void EndFrame() override;

        void BeginScene(const Camera& camera) override;
        void EndScene() override;


        void BeginRenderPass(Renderer::ISwapChain& swapChain, const char* name) override;
        void BeginRenderPass(const RenderPassDescriptor& desc, const char* name) override;
        void EndRenderPass() override;
        
        void CopyBuffer(BufferId srcBuffId, uint32_t srcIndex, BufferId dstBuffId, uint32_t dstIndex, uint32_t size) override;

        void ReleaseBuffer(BufferId bufferId) override;
        void ReleaseTexture(TextureId textureId) override;
        void ReleaseSampler(SamplerId samplerId) override;
        void ReleasePipeline(PipelineId pipelineId) override;
        
    private:
        std::vector<ResourceId> ProcessStage(PipelineId id, const ModuleDescriptor& module, const std::vector<std::string>& functions, MTL::RenderPipelineDescriptor& desc);
        
    private:
        std::shared_ptr<MTL::MetalDevice> mDevice;
        MTL::CommandQueue mCommandQueue;
        MTL::CommandQueue mTransferQueue;
        
        std::unique_ptr<MTL::Library> mDefaultLibrary;
        
        std::vector<MTL::RenderPipelineState> mRenderPipelines;
        std::vector<MTL::Buffer> mBuffers;
        std::vector<MTL::Texture> mTextures;
        std::vector<MTL::SamplerState> mSamplers;
        std::vector<MTL::DepthStencilState> mDepthStencilStates;
        std::vector<MTL::Texture> mDepthTextures;
        
        MTL::Heap mPrivateHeap;
        
        MTL::CommandBuffer mFrameCmdBuffer;
        dispatch_semaphore_t mFrameCmdBufferSemaphore;
        
        RenderPass mRenderPass;
        
        std::deque<uint32_t> mBufferFreeList;
        
        std::unordered_map<PipelineId, PipelineState> mPipelineStateCache;
        
        uint32_t mRenderPassIndex{ 1 };

        BufferId mVertUniBufferId;
        BufferId mFragUniBufferId;

        // Shared data for shaders
        VertexBufferSharedData mSceneVertexData;
        FragmentBufferSharedData mSceneFragData;
    };
}
