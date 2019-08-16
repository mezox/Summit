#include "pch.h"

#include <MetalCpp/MetalCommandQueue.h>
#include <MetalCpp/MetalCommandBuffer.h>

namespace MTL
{
    CommandQueue::CommandQueue(void* handle)
        : NSHandleWrapper(handle)
    {}

    CommandBuffer CommandQueue::CreateCommandBuffer() const noexcept
    {
        id<MTLCommandBuffer> buffer = [(__bridge id<MTLCommandQueue>)mNativeHandle commandBuffer];
        return CommandBuffer((__bridge void*)buffer);
    }
    
    CommandBuffer CommandQueue::CommandBufferWithUnretainedReferences() const noexcept
    {
        id<MTLCommandBuffer> buffer = [(__bridge id<MTLCommandQueue>)mNativeHandle commandBufferWithUnretainedReferences];
        return CommandBuffer((__bridge void*)buffer);
    }
}
