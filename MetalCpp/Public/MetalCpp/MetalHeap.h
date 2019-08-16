#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalDeviceResource.h"
#include "MetalResource.h"

namespace MTL
{
    class Buffer;
    
    class METAL_CPP_API HeapDescriptor final : public NSHandleWrapper
    {
        friend class MetalDevice;
        
    public:
        HeapDescriptor();
        
        /*!
         @abstract Requested size of the heap's backing memory.
         @discussion The size may be rounded up to GPU page granularity.
         */
        void SetSize(uint32_t size) noexcept;
        
        /*!
         @abstract Storage mode for the heap. Default is MTLStorageModePrivate.
         @discussion All resources created from this heap share the same storage mode.
         MTLStorageModeManaged and MTLStorageModeMemoryless are disallowed.
         */
        void SetStorageMode(StorageMode mode) noexcept;
        
        /*!
         @abstract CPU cache mode for the heap. Default is MTLCPUCacheModeDefaultCache.
         @discussion All resources created from this heap share the same cache mode.
         CPU cache mode is ignored for MTLStorageModePrivate.
         */
        void SetCPUCacheMode(CPUCacheMode mode) noexcept;
    };
    
    
    class METAL_CPP_API Heap final : public NSHandleWrapper, public DeviceResource<Heap>
    {
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        
    public:
        Heap() = default;
        
        explicit Heap(void* handle);
        
        /*!
         @abstract Current heap storage mode, default is MTLStorageModePrivate.
         @discussion All resources created from this heap share the same storage mode.
         */
        [[nodiscard]] StorageMode GetStorageMode() const noexcept;
        
        /*!
         @abstract CPU cache mode for the heap. Default is MTLCPUCacheModeDefaultCache.
         @discussion All resources created from this heap share the same cache mode.
         */
        [[nodiscard]] CPUCacheMode GetCpuCacheMode() const noexcept;
        
        /*!
         @abstract Heap size in bytes, specified at creation time and rounded up to device specific alignment.
         */
        [[nodiscard]] uint32_t GetSize() const noexcept;
        
        /*!
         @abstract The size in bytes, of all resources allocated from the heap.
         */
        [[nodiscard]] uint32_t GetUsedSize() const noexcept;
        
        /*!
         @abstract Create a new buffer backed by heap memory.
         @discussion The requested storage and CPU cache modes must match the storage and CPU cache modes of the heap.
         @return The buffer or nil if heap is full.
         */
        [[nodiscard]] Buffer NewBuffer(uint32_t length, ResourceOptions options) const noexcept;
    };
}
