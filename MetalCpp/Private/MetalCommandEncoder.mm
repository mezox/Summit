#include "pch.h"

#include <MetalCpp/MetalCommandEncoder.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalBuffer.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalSampler.h>
#include <MetalCpp/MetalDepthStencil.h>
#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalHeap.h>

#include "MetalTypeConverter.h"

namespace MTL
{    
    CommandEncoder::CommandEncoder(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    void CommandEncoder::EndEncoding() const noexcept
    {
        [(__bridge id<MTLCommandEncoder>)mNativeHandle endEncoding];
    }
    
    void CommandEncoder::InsertDebugSignpost(const char* string) const noexcept
    {
        [(__bridge id<MTLCommandEncoder>)mNativeHandle insertDebugSignpost:[NSString stringWithCString:string encoding:NSASCIIStringEncoding]];
    }

    void CommandEncoder::PushDebugGroup(const char* string) const noexcept
    {
        [(__bridge id<MTLCommandEncoder>)mNativeHandle pushDebugGroup:[NSString stringWithCString:string encoding:NSASCIIStringEncoding]];
    }
    
    void CommandEncoder::PopDebugGroup() const noexcept
    {
        [(__bridge id<MTLCommandEncoder>)mNativeHandle popDebugGroup];
    }
    
    RenderCommandEncoder::RenderCommandEncoder(void* handle)
        : CommandEncoder(handle)
    {}
    
    void RenderCommandEncoder::SetRenderPipelineState(const RenderPipelineState& state) const
    {
        const auto nativeState = (__bridge id<MTLRenderPipelineState>)state.mNativeHandle;
        [(__bridge id<MTLRenderCommandEncoder>)mNativeHandle setRenderPipelineState:nativeState];
    }
    
    void RenderCommandEncoder::SetViewport(const Viewport& viewport) const noexcept
    {
        @autoreleasepool
        {
            MTLViewport nativeViewport;
            nativeViewport.originX = viewport.originX;
            nativeViewport.originY = viewport.originY;
            nativeViewport.width = viewport.width;
            nativeViewport.height = viewport.height;
            nativeViewport.znear = viewport.znear;
            nativeViewport.zfar = viewport.zfar;
            
            [(__bridge id<MTLRenderCommandEncoder>)GetPtr() setViewport:nativeViewport];
        }
    }
    
    void RenderCommandEncoder::SetScissorRect(const ScissorRect& rect) const noexcept
    {
        @autoreleasepool
        {
            MTLScissorRect nativeScissorRect;
            nativeScissorRect.x = rect.x;
            nativeScissorRect.y = rect.y;
            nativeScissorRect.width = rect.width;
            nativeScissorRect.height = rect.height;
            
            [(__bridge id<MTLRenderCommandEncoder>)GetPtr() setScissorRect:nativeScissorRect];
        }
    }
    
    void RenderCommandEncoder::SetVertexBufferOffset(uint32_t offset, uint32_t index) const noexcept
    {
        [(__bridge id<MTLRenderCommandEncoder>)GetPtr() setVertexBufferOffset:offset atIndex:index];
    }
    
    void RenderCommandEncoder::SetDepthStencilState(const DepthStencilState& depthStencilState) const noexcept
    {
        const auto nativeState = (__bridge id<MTLDepthStencilState>)depthStencilState.mNativeHandle;
        [(__bridge id<MTLRenderCommandEncoder>)mNativeHandle setDepthStencilState:nativeState];
    }
    
    void RenderCommandEncoder::SetVertexBuffer(const Buffer& buffer, const uint32_t offset, const uint32_t index) const
    {
        const auto nativeBuffer = (__bridge id<MTLBuffer>)buffer.mNativeHandle;
        [(__bridge id<MTLRenderCommandEncoder>)mNativeHandle setVertexBuffer:nativeBuffer offset:offset atIndex:index];
    }
    
    void RenderCommandEncoder::SetVertexBytes(const void* bytes, const uint32_t length, const uint32_t index) const noexcept
    {
        auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        [nativeEncoder setVertexBytes:bytes length:length atIndex:index];
    }
    
    void RenderCommandEncoder::DrawPrimitives(const PrimitiveType type, const uint32_t vertexStart, const uint32_t vertexCount) const
    {
        [(__bridge id<MTLRenderCommandEncoder>)mNativeHandle drawPrimitives:ConvertType(type) vertexStart:vertexStart vertexCount:vertexCount];
    }
    
    void RenderCommandEncoder::DrawIndexedPrimitives(const PrimitiveType type, const uint32_t indexCount, const IndexType indexType, const Buffer& indexBuffer, const uint32_t indexBufferOffset) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        const auto nativeIndexBuffer = (__bridge id<MTLBuffer>)indexBuffer.mNativeHandle;
        
        [nativeEncoder drawIndexedPrimitives:ConvertType(type)
                                  indexCount:indexCount
                                   indexType:ConvertType(indexType)
                                 indexBuffer:nativeIndexBuffer
                           indexBufferOffset:indexBufferOffset];
    }
    
    void RenderCommandEncoder::DrawIndexedPrimitives(const PrimitiveType type, const uint32_t indexCount, const IndexType indexType, const Buffer& indexBuffer, const uint32_t indexBufferOffset, const uint32_t instanceCount) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)GetPtr();
        const auto nativeIndexBuffer = (__bridge id<MTLBuffer>)indexBuffer.GetPtr();
        
        [nativeEncoder drawIndexedPrimitives:ConvertType(type)
                                  indexCount:indexCount
                                   indexType:ConvertType(indexType)
                                 indexBuffer:nativeIndexBuffer
                           indexBufferOffset:indexBufferOffset
                               instanceCount:instanceCount];
    }
    
    void RenderCommandEncoder::SetFragmentBuffer(const Buffer& buffer, const uint32_t offset, const uint32_t index) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        const auto nativeUniformBuffer = (__bridge id<MTLBuffer>)buffer.mNativeHandle;
        
        [nativeEncoder setFragmentBuffer:nativeUniformBuffer offset:offset atIndex:index];
    }
    
    void RenderCommandEncoder::SetFragmentTexture(const Texture& texture, const uint32_t index) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        const auto nativeTexture = (__bridge id<MTLTexture>)texture.mNativeHandle;
        [nativeEncoder setFragmentTexture:nativeTexture atIndex:index];
    }
    
    void RenderCommandEncoder::SetFragmentSamplerState(const SamplerState& samplerState, const uint32_t index) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        const auto nativeSampler = (__bridge id<MTLSamplerState>)samplerState.mNativeHandle;
        [nativeEncoder setFragmentSamplerState:nativeSampler atIndex:index];
    }
    
    void RenderCommandEncoder::SetFrontFacingWinding(Winding frontFacingWinding) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        [nativeEncoder setFrontFacingWinding:ConvertType(frontFacingWinding)];
    }
    
    void RenderCommandEncoder::SetCullMode(CullMode cullMode) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        [nativeEncoder setCullMode:ConvertType(cullMode)];
    }
    
    void RenderCommandEncoder::UseResource(const void* resource, ResourceUsage usage) const noexcept
    {
        auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        auto nativeResource = (__bridge id<MTLResource>)resource;
        [nativeEncoder useResource:nativeResource usage:ConvertType(usage)];
    }
    
    void RenderCommandEncoder::UseHeap(const Heap& heap) const noexcept
    {
        const auto nativeEncoder = (__bridge id<MTLRenderCommandEncoder>)mNativeHandle;
        [nativeEncoder useHeap:(__bridge id<MTLHeap>)heap.GetPtr()];
    }
    
    BlitEncoder::BlitEncoder(void* handle)
        : CommandEncoder(handle)
    {}
    
    void BlitEncoder::CopyFromBuffer(const Buffer& sourceBuffer, uint32_t sourceOffset, const Buffer& destinationBuffer, uint32_t destinationOffset, uint32_t size) const noexcept
    {
        id<MTLBuffer> sourceNativeBuffer = (__bridge id<MTLBuffer>)sourceBuffer.mNativeHandle;
        id<MTLBuffer> destNativeBuffer = (__bridge id<MTLBuffer>)destinationBuffer.mNativeHandle;
        id<MTLBlitCommandEncoder> nativeEncoder = (__bridge id<MTLBlitCommandEncoder>)mNativeHandle;
        [nativeEncoder copyFromBuffer:sourceNativeBuffer sourceOffset:sourceOffset toBuffer:destNativeBuffer destinationOffset:destinationOffset size:size];
    }
    
    void BlitEncoder::GenerateMipmaps(const Texture& texture) const noexcept
    {
        id<MTLBlitCommandEncoder> nativeEncoder = (__bridge id<MTLBlitCommandEncoder>)GetPtr();
        id<MTLTexture> nativeTexture = (__bridge id<MTLTexture>)texture.GetPtr();
        [nativeEncoder generateMipmapsForTexture:nativeTexture];
    }
    
    IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(CommandEncoder)
}
