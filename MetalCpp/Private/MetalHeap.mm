#include <pch.h>

#include <MetalCpp/MetalHeap.h>
#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalBuffer.h>

#include "MetalTypeConverter.h"

namespace MTL
{
    HeapDescriptor::HeapDescriptor()
        : NSHandleWrapper((__bridge void*)[[MTLHeapDescriptor new] autorelease])
    {}
    
    void HeapDescriptor::SetSize(const uint32_t size) noexcept
    {
        [(__bridge MTLHeapDescriptor*)mNativeHandle setSize:size];
    }
    
    void HeapDescriptor::SetStorageMode(const StorageMode mode) noexcept
    {
        [(__bridge MTLHeapDescriptor*)mNativeHandle setStorageMode:ConvertType(mode)];
    }
    
    void HeapDescriptor::SetCPUCacheMode(const CPUCacheMode mode) noexcept
    {
        [(__bridge MTLHeapDescriptor*)mNativeHandle setCpuCacheMode:ConvertType(mode)];
    }
    
    Heap::Heap(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    StorageMode Heap::GetStorageMode() const noexcept
    {
        return ConvertType([(__bridge id<MTLHeap>)mNativeHandle storageMode]);
    }
    
    CPUCacheMode Heap::GetCpuCacheMode() const noexcept
    {
        return ConvertType([(__bridge id<MTLHeap>)mNativeHandle cpuCacheMode]);
    }
    
    uint32_t Heap::GetSize() const noexcept
    {
        return [(__bridge id<MTLHeap>)mNativeHandle size];
    }
    
    uint32_t Heap::GetUsedSize() const noexcept
    {
        return [(__bridge id<MTLHeap>)mNativeHandle usedSize];
    }
    
    Buffer Heap::NewBuffer(const uint32_t length, ResourceOptions options) const noexcept
    {
        return Buffer((__bridge void*)[(__bridge id<MTLHeap>)mNativeHandle newBufferWithLength:length options:ConvertType(options)]);
    }
}
