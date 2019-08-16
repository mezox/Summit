#include "pch.h"

#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalBuffer.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalSampler.h>
#include <MetalCpp/MetalDepthStencil.h>
#include <MetalCpp/MetalHeap.h>

#include "MetalTypeConverter.h"

namespace MTL
{
    MetalDevice::MetalDevice(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    std::string MetalDevice::GetDeviceName() const noexcept
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        return [[device name] cStringUsingEncoding:NSASCIIStringEncoding];
    }
    
    Heap MetalDevice::NewHeap(const HeapDescriptor& descriptor) const noexcept
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        MTLHeapDescriptor* nativeHeapDesc = (__bridge MTLHeapDescriptor*)descriptor.mNativeHandle;
        return Heap((__bridge void*)[device newHeapWithDescriptor:nativeHeapDesc]);
    }
    
    CommandQueue MetalDevice::NewCommandQueue(const uint32_t maxCmdBuffers)
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        id<MTLCommandQueue> cmdQueue = nil;
        
        if(maxCmdBuffers == 1)
        {
            cmdQueue = [device newCommandQueue];
        }
        else
        {
            cmdQueue = [device newCommandQueueWithMaxCommandBufferCount:maxCmdBuffers];
        }
        
        if(cmdQueue == nil)
        {
            throw std::runtime_error("Failed to create command queue");
        }
        
        return CommandQueue((__bridge void*)cmdQueue);
    }
    
    uint32_t MetalDevice::GetCurrentAllocatedSize() const noexcept
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        return device.currentAllocatedSize;
    }
    
    Library MetalDevice::CreateNewDefaultLibrary() const noexcept
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        return Library((__bridge void*)[device newDefaultLibrary]);
    }
    
    Library MetalDevice::NewLibraryWithSource(const std::string& source) const
    {
        id<MTLDevice> device = (__bridge id<MTLDevice>)mNativeHandle;
        NSString* librarySource = [NSString stringWithCString:source.c_str() encoding:NSASCIIStringEncoding];
        NSError* error = nil;
        
        id<MTLLibrary> library = [device newLibraryWithSource:librarySource options:nil error:&error];
        if(error)
        {
            NSLog(@"%@",[error localizedDescription]);
            throw std::runtime_error("Failed to create library from source");
        }
        
        return Library((__bridge void*)library);
    }
    
    RenderPipelineState MetalDevice::NewRenderPipelineState(const RenderPipelineDescriptor& descriptor) const
    {
        NSError* nsError{ nil };
        
        auto nativeDevicePtr = (__bridge id<MTLDevice>)mNativeHandle;
        auto renderPipelineState = [nativeDevicePtr newRenderPipelineStateWithDescriptor:(__bridge MTLRenderPipelineDescriptor*)descriptor.mNativeHandle
                                                                                                                       error:&nsError];
        if(nsError)
        {
            NSLog(@"%@",[nsError localizedDescription]);
            throw std::runtime_error("Failed to create renderPipelineState");
        }

        return RenderPipelineState((__bridge void*)renderPipelineState);
    }
    
    Buffer MetalDevice::NewBuffer(const void *pointer, const uint32_t length, ResourceOptions options) const
    {
        const auto device = (__bridge id<MTLDevice>)mNativeHandle;
        return Buffer((__bridge void*)[device newBufferWithBytes:pointer length:length options:ConvertType(options)]);
    }
    
    Buffer MetalDevice::NewBuffer(const uint32_t length, ResourceOptions options) const
    {
        const auto device = (__bridge id<MTLDevice>)mNativeHandle;
        return Buffer((__bridge void*)[device newBufferWithLength:length options:ConvertType(options)]);
    }
    
    DepthStencilState MetalDevice::NewDepthStencilState(const DepthStencilDescriptor& desc) const
    {
        const auto nativeDevice = (__bridge id<MTLDevice>)mNativeHandle;
        const auto nativeDSDesc = (__bridge MTLDepthStencilDescriptor*)desc.mNativeHandle;
        return DepthStencilState((__bridge void*)[nativeDevice newDepthStencilStateWithDescriptor:nativeDSDesc]);
    }
    
    Texture MetalDevice::NewTexture(const TextureDescriptor& descriptor) const
    {
        auto nativeDevice = (__bridge id<MTLDevice>)mNativeHandle;
        auto nativeTexDesc = (__bridge MTLTextureDescriptor*)descriptor.mNativeHandle;
        return Texture((__bridge void*)[nativeDevice newTextureWithDescriptor:nativeTexDesc]);
    }
    
    SamplerState MetalDevice::NewSamplerState(const SamplerDescriptor& descriptor) const
    {
        auto nativeDevice = (__bridge id<MTLDevice>)mNativeHandle;
        auto nativeSamplerDesc = (__bridge MTLSamplerDescriptor*)descriptor.mNativeHandle;
        return SamplerState((__bridge void*)[nativeDevice newSamplerStateWithDescriptor:nativeSamplerDesc]);
    }
    
    MetalDevice CreateSystemDefaultDevice()
    {
        return MetalDevice((__bridge void*)MTLCreateSystemDefaultDevice());
    }
}
