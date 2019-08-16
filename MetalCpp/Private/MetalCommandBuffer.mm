#include "pch.h"

#include <MetalCpp/MetalCommandBuffer.h>
#include <MetalCpp/MetalCommandEncoder.h>
#include <MetalCpp/MetalRenderPass.h>
#include <MetalCpp/MetalDrawable.h>

namespace MTL
{
    CommandBuffer::CommandBuffer(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    bool CommandBuffer::HoldsRetainedReferences() const noexcept
    {
        id<MTLCommandBuffer> cmdBuff = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        return cmdBuff.retainedReferences;
    }
    
    void CommandBuffer::Commit() const
    {
        id<MTLCommandBuffer> cmdBuff = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        [cmdBuff commit];
    }
    
    void CommandBuffer::Enqueue() const
    {
        id<MTLCommandBuffer> cmdBuff = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        [cmdBuff enqueue];
    }
    
    void CommandBuffer::Present(const Drawable& drawable) const
    {
        [(__bridge id<MTLCommandBuffer>)mNativeHandle presentDrawable:(__bridge id<MTLDrawable>)drawable.mNativeHandle];
    }
    
    RenderCommandEncoder CommandBuffer::CreateRenderCommandEncoder(const RenderPassDescriptor& desc) const noexcept
    {
        auto commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        auto renderCommandEncoder = [commandBuffer renderCommandEncoderWithDescriptor:(__bridge MTLRenderPassDescriptor*)desc.mNativeHandle];
        return RenderCommandEncoder((__bridge void*)renderCommandEncoder);
    }
    
    void CommandBuffer::WaitUntilScheduled() const noexcept
    {
        auto commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        [commandBuffer waitUntilScheduled];
    }
    
    void CommandBuffer::WaitUntilCompleted() const noexcept
    {
        auto commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        [commandBuffer waitUntilCompleted];
    }
    
    BlitEncoder CommandBuffer::BlitCommandEncoder() const noexcept
    {
        auto commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        return BlitEncoder((__bridge void*)[commandBuffer blitCommandEncoder]);
    }
    
    
    double CommandBuffer::GetGPUStartTime() const noexcept
    {
        //return [(__bridge id<MTLCommandBuffer>)mNativeHandle GPUStartTime];
        return 0.0;
    }
    
    double CommandBuffer::GetGPUEndTime() const noexcept
    {
        //return [(__bridge id<MTLCommandBuffer>)mNativeHandle GPUEndTime]; // only ios
        return 0.0;
    }
    
    void CommandBuffer::AddScheduledHandler(CommandBufferHandler handler) const noexcept
    {
        id<MTLCommandBuffer> commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;
        [commandBuffer addScheduledHandler:^(id <MTLCommandBuffer> mtlCommandBuffer) {
            CommandBuffer commandBuffer((__bridge void*)mtlCommandBuffer);
            handler(commandBuffer);
        }];
    }
    
    void CommandBuffer::AddCompletedHandler(CommandBufferHandler handler) const noexcept
    {
        id<MTLCommandBuffer> commandBuffer = (__bridge id<MTLCommandBuffer>)mNativeHandle;        
        [commandBuffer addCompletedHandler:^(id <MTLCommandBuffer> mtlCommandBuffer) {
            CommandBuffer commandBuffer((__bridge void*)mtlCommandBuffer);
            handler(commandBuffer);
        }];
    }
    
    IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(CommandBuffer)
}
