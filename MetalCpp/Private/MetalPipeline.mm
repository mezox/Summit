#include "pch.h"

#include <MetalCpp/MetalBuffer.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalSampler.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalVertexDescriptor.h>
#include <MetalCpp/MetalTypes.h>

#include "MetalTypeConverter.h"

namespace MTL
{
    Function::Function(void* handle) noexcept
        : NSHandleWrapper(handle)
    {}
    
    FunctionType Function::GetFunctionType() const noexcept
    {
        return ConvertType([(__bridge id<MTLFunction>)GetPtr() functionType]);
    }
    
    ArgumentEncoder Function::NewArgumentEncoder(uint32_t bufferIndex) const noexcept
    {
        return ArgumentEncoder((__bridge void*)[(__bridge id<MTLFunction>)GetPtr() newArgumentEncoderWithBufferIndex:bufferIndex]);
    }
    
    ArgumentEncoder::ArgumentEncoder(void* handle) noexcept
        : NSHandleWrapper(handle)
    {}
    
    uint32_t ArgumentEncoder::GetEncodedLength() const noexcept
    {
        return static_cast<uint32_t>([(__bridge id<MTLArgumentEncoder>)GetPtr() encodedLength]);
    }
    
    uint32_t ArgumentEncoder::GetAlignment() const noexcept
    {
        return static_cast<uint32_t>([(__bridge id<MTLArgumentEncoder>)GetPtr() alignment]);
    }
    
    void ArgumentEncoder::SetArgumentBuffer(const Buffer& argumentBuffer, uint32_t offset) const noexcept
    {
        id<MTLBuffer> nativeBuffer = (__bridge id<MTLBuffer>)argumentBuffer.GetPtr();
        [(__bridge id<MTLArgumentEncoder>)GetPtr() setArgumentBuffer:nativeBuffer offset:offset];
    }
    
    void ArgumentEncoder::SetTexture(const Texture& texture, uint32_t index) const noexcept
    {
        id<MTLTexture> nativeTexture = (__bridge id<MTLTexture>)texture.GetPtr();
        [(__bridge id<MTLArgumentEncoder>)GetPtr() setTexture:nativeTexture atIndex:index];
    }
    
    void ArgumentEncoder::SetSamplerState(const SamplerState& state, uint32_t index) const noexcept
    {
        id<MTLSamplerState> nativeSamplerState = (__bridge id<MTLSamplerState>)state.GetPtr();
        [(__bridge id<MTLArgumentEncoder>)GetPtr() setSamplerState:nativeSamplerState atIndex:index];
    }
    
    void* ArgumentEncoder::GetConstantData(uint32_t index) const noexcept
    {
        return [(__bridge id<MTLArgumentEncoder>)GetPtr() constantDataAtIndex:index];
    }
    
    IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(ArgumentEncoder);
    
    Library::Library(void* handle)
        : mNativeHandle(handle)
    {}
    
    Library::~Library()
    {
        mNativeHandle = nullptr;
    }
    
    Function Library::CreateNewFunctionWithName(const std::string& functionName) const
    {
        const auto functionPtr = [(__bridge id<MTLLibrary>)mNativeHandle newFunctionWithName:[NSString stringWithUTF8String:functionName.c_str()]];
        if(!functionPtr)
        {
            throw std::runtime_error("FunctionNotFound");
        }
        
        return Function((__bridge void*)functionPtr);
    }
    
    std::vector<std::string> Library::GetFunctionNames() const noexcept
    {
        id<MTLLibrary> nativeLibrary = (__bridge id<MTLLibrary>)mNativeHandle;
        NSArray<NSString*>* nativeFunctionNames = [nativeLibrary functionNames];
        
        std::vector<std::string> cppNames;
        cppNames.reserve([nativeFunctionNames count]);
        
        for(const auto name in nativeFunctionNames)
        {
            cppNames.push_back([name cStringUsingEncoding:NSASCIIStringEncoding]);
        }
        
        return cppNames;
    }
    
    RenderPipelineColorAttachmentDescriptor::RenderPipelineColorAttachmentDescriptor()
        : mNativeHandle((__bridge void*)[[MTLRenderPipelineColorAttachmentDescriptor alloc] init])
    {}
    
    RenderPipelineColorAttachmentDescriptor::RenderPipelineColorAttachmentDescriptor(void* handle)
        : mNativeHandle(handle)
    {}
    
    RenderPipelineColorAttachmentDescriptor::~RenderPipelineColorAttachmentDescriptor()
    {
        mNativeHandle = nullptr;
    }
    
    void RenderPipelineColorAttachmentDescriptor::EnableBlending(bool toggle) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setBlendingEnabled:toggle];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetSourceRGBBlendFactor(BlendFactor factor) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setSourceRGBBlendFactor:ConvertType(factor)];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetDestinationRGBBlendFactor(BlendFactor factor) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setDestinationRGBBlendFactor:ConvertType(factor)];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetRGBBlendOperation(BlendOperation op) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setRgbBlendOperation:ConvertType(op)];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetSourceAlphaBlendFactor(BlendFactor factor) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setSourceAlphaBlendFactor:ConvertType(factor)];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetDestinationAlphaBlendFactor(BlendFactor factor) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setDestinationAlphaBlendFactor:ConvertType(factor)];
    }
    
    void RenderPipelineColorAttachmentDescriptor::SetAlphaBlendOperation(BlendOperation op) noexcept
    {
        [(__bridge MTLRenderPipelineColorAttachmentDescriptor*)mNativeHandle setAlphaBlendOperation:ConvertType(op)];
    }
    
    // ---------- RenderPipelineDescriptor ------------------
    RenderPipelineDescriptor::RenderPipelineDescriptor()
        : NSHandleWrapper((__bridge void*)[[MTLRenderPipelineDescriptor new] autorelease])
    {}
    
    RenderPipelineDescriptor::RenderPipelineDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    void RenderPipelineDescriptor::SetLabel(const std::string& label) const
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        [nativeDesc setLabel:[NSString stringWithCString:label.c_str() encoding:NSASCIIStringEncoding]];
    }
    
    void RenderPipelineDescriptor::SetVertexFunction(const Function& f) const
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        auto nativeFunc = (__bridge id<MTLFunction>)f.mNativeHandle;
        [nativeDesc setVertexFunction: nativeFunc];
    }
    
    void RenderPipelineDescriptor::SetFragmentFunction(const Function& f) const
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        auto nativeFunc = (__bridge id<MTLFunction>)f.mNativeHandle;
        [nativeDesc setFragmentFunction: nativeFunc];
        
        // TODO: HACK
        nativeDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    }
    
    void RenderPipelineDescriptor::SetVertexDescriptor(const VertexDescriptor& desc) const noexcept
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        auto nativeVertexDesc = (__bridge MTLVertexDescriptor*)desc.mNativeHandle;
        [nativeDesc setVertexDescriptor:nativeVertexDesc];
    }
    
    void RenderPipelineDescriptor::SetDepthAttachmentPixelFormat(const PixelFormat format) const noexcept
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        [nativeDesc setDepthAttachmentPixelFormat:ConvertType(format)];
    }
    
    void RenderPipelineDescriptor::SetStencilAttachmentPixelFormat(const PixelFormat format) const noexcept
    {
        auto nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        [nativeDesc setStencilAttachmentPixelFormat:ConvertType(format)];
    }
    
    MTLArrayWrapper<RenderPipelineColorAttachmentDescriptor> RenderPipelineDescriptor::GetAttachments() const noexcept
    {
        MTLRenderPipelineDescriptor* nativeDesc = (__bridge MTLRenderPipelineDescriptor*)mNativeHandle;
        return MTLArrayWrapper<RenderPipelineColorAttachmentDescriptor>((__bridge void*)nativeDesc.colorAttachments);
    }
    
    // ---------- RenderPipelineDescriptor ------------------
    
    
    // ---------- RenderPipelineState ------------------
    
    RenderPipelineState::RenderPipelineState(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    // ---------- RenderPipelineState ------------------
}
