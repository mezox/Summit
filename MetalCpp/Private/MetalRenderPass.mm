#include "pch.h"

#include <MetalCpp/MetalRenderPass.h>
#include <MetalCpp/MetalTexture.h>
#include "MetalTypeConverter.h"

namespace MTL
{
    RenderPassAttachmentDescriptor::RenderPassAttachmentDescriptor()
        : NSHandleWrapper((__bridge void*)[[[MTLRenderPassAttachmentDescriptor alloc] init] autorelease])
    {}
    
    RenderPassAttachmentDescriptor::RenderPassAttachmentDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    
    void RenderPassAttachmentDescriptor::SetTexture(const Texture& texture)
    {
        id<MTLTexture> nativeTexture = (__bridge id<MTLTexture>)texture.mNativeHandle;
        auto format = nativeTexture.pixelFormat;
        return [(__bridge MTLRenderPassAttachmentDescriptor*)mNativeHandle setTexture:nativeTexture];
    }
    
    void RenderPassAttachmentDescriptor::SetLoadAction(LoadAction action)
    {
        return [(__bridge MTLRenderPassAttachmentDescriptor*)mNativeHandle setLoadAction:ConvertType(action)];
    }
    
    void RenderPassAttachmentDescriptor::SetStoreAction(StoreAction action)
    {
        return [(__bridge MTLRenderPassAttachmentDescriptor*)mNativeHandle setStoreAction:ConvertType(action)];
    }
    
    RenderPassColorAttachmentDescriptor::RenderPassColorAttachmentDescriptor(void* handle)
        : RenderPassAttachmentDescriptor(handle)
    {}
    
    void RenderPassColorAttachmentDescriptor::SetClearColor(const ClearColor& color)
    {
        return [(__bridge MTLRenderPassColorAttachmentDescriptor*)mNativeHandle setClearColor:MTLClearColorMake(color.red, color.green, color.blue, color.alpha)];
    }
    
    RenderPassDepthAttachmentDescriptor::RenderPassDepthAttachmentDescriptor(void* handle)
        : RenderPassAttachmentDescriptor(handle)
    {}
    
    void RenderPassDepthAttachmentDescriptor::SetClearDepth(double value)
    {
        return [(__bridge MTLRenderPassDepthAttachmentDescriptor*)mNativeHandle setClearDepth:value];
    }
    
    RenderPassDescriptor::RenderPassDescriptor()
        : NSHandleWrapper((__bridge void*)[MTLRenderPassDescriptor renderPassDescriptor])
    {}
    
    RenderPassDescriptor::RenderPassDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    MTLArrayWrapper<RenderPassColorAttachmentDescriptor> RenderPassDescriptor::GetColorAttachments() const noexcept
    {
        MTLRenderPassDescriptor* nativeDesc = (__bridge MTLRenderPassDescriptor*)mNativeHandle;
        return MTLArrayWrapper<RenderPassColorAttachmentDescriptor>((__bridge void*)nativeDesc.colorAttachments);
    }
    
    RenderPassDepthAttachmentDescriptor RenderPassDescriptor::GetDepthAttachment() const noexcept
    {
        MTLRenderPassDescriptor* nativeDesc = (__bridge MTLRenderPassDescriptor*)mNativeHandle;
        return RenderPassDepthAttachmentDescriptor((__bridge void*)nativeDesc.depthAttachment);
    }
}
