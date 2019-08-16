#pragma once

#include "MetalDeviceResource.h"

namespace MTL
{
    /*!
     @brief Describes what CPU cache mode is used for the CPU's mapping of a texture resource.

     @discussion
     Applications should only investigate changing the cache mode if writing to normally cached buffers is known to cause performance issues due to cache pollution, as write combined memory can have surprising performance pitfalls.  Another approach is to use non-temporal stores to normally cached memory (STNP on ARMv8, _mm_stream_* on x86_64).
     */
    enum class CPUCacheMode
    {
        /*
         The default cache mode for the system.
         */
        DefaultCache = 0,
        
        /*
         Write combined memory is optimized for resources that the CPU will write into, but never read.  On some implementations, writes may bypass caches avoiding cache pollution, and reads perform very poorly.
         */
        WriteCombined = 1
    };
    
    /*!
     @abstract Describes location and CPU mapping of MTLTexture.
     */
    enum class StorageMode
    {
        /*
         In this mode, CPU and device will nominally both use the same underlying memory when accessing the contents of the texture resource.
         However, coherency is only guaranteed at command buffer boundaries to minimize the required flushing of CPU and GPU caches.
         This is the default storage mode for iOS Textures.
         */
        Shared = 0,
        
        /*
         This mode relaxes the coherency requirements and requires that the developer make explicit requests to maintain
         coherency between a CPU and GPU version of the texture resource.  In order for CPU to access up to date GPU results,
         first, a blit synchronizations must be completed (see synchronize methods of MTLBlitCommandEncoder).
         Blit overhead is only incurred if GPU has modified the resource.
         This is the default storage mode for OS X Textures.
         */
        Managed = 1,
        
        /*
         This mode allows the texture resource data to be kept entirely to GPU (or driver) private memory that will never be accessed by the CPU directly, so no
         conherency of any kind must be maintained.
         */
        Private = 2,
        
        /*
         This mode allows creation of resources that do not have a GPU or CPU memory backing, but do have on-chip storage for TBDR
         devices. The contents of the on-chip storage is undefined and does not persist, but its configuration is controlled by the
         MTLTexture descriptor. Textures created with MTLStorageModeMemoryless dont have an IOAccelResource at any point in their
         lifetime. The only way to populate such resource is to perform rendering operations on it. Blit operations are disallowed.
         */
        Memoryless = 3
    };
    
    /*!
     @brief Options for setPurgeable call.
     */
    enum class PurgeableState
    {
        /*
         The purgeabelity state is not changed.
         */
        KeepCurrent,
        
        /*
         The contents of this resource may not be discarded.
         */
        NonVolatile,
        
        /*
         The contents of this resource may be discarded.
         */
        Volatile,
        
        /*
         The contents of this are discarded.
         */
        Empty
    };
    
    static constexpr uint32_t ResourceCPUCacheModeShift = 0;
    static constexpr uint32_t ResourceStorageModeShift = 4;
    static constexpr uint32_t ResourceHazardTrackingModeShift = 8;
    
    static constexpr uint32_t ResourceCPUCacheModeMask = (0xfUL << ResourceCPUCacheModeShift);
    static constexpr uint32_t ResourceStorageModeMask = (0xfUL << ResourceStorageModeShift);
    static constexpr uint32_t ResourceHazardTrackingModeMask = (0x1UL << ResourceHazardTrackingModeShift);
    
    enum class ResourceOptions
    {
        CPUCacheModeDefaultCache  = uint32_t(CPUCacheMode::DefaultCache)  << ResourceCPUCacheModeShift,
        CPUCacheModeWriteCombined = uint32_t(CPUCacheMode::WriteCombined) << ResourceCPUCacheModeShift,
        
        StorageModeShared       = uint32_t(StorageMode::Shared)     << ResourceStorageModeShift,
        StorageModeManaged      = uint32_t(StorageMode::Managed)    << ResourceStorageModeShift,
        StorageModePrivate      = uint32_t(StorageMode::Private)    << ResourceStorageModeShift,
        //StorageModeMemoryless   = uint32_t(StorageMode::Memoryless) << ResourceStorageModeShift,
        
        HazardTrackingModeUntracked = 0x1UL << ResourceHazardTrackingModeShift,
        
        // Deprecated spellings
        //OptionCPUCacheModeDefault       = CPUCacheModeDefaultCache,
        //OptionCPUCacheModeWriteCombined = CPUCacheModeWriteCombined,
    };
    
    /*!
     @brief Common APIs available for MTLBuffer and MTLTexture instances
     */
    template<typename D>
    class Resource : public DeviceResource<D>
    {
    public:
        /*!
         @brief The cache mode used for the CPU mapping for this resource
         */
        [[nodiscard]] CPUCacheMode GetCpuCacheMode() const noexcept
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.GetCpuCacheMode();
        }
        
        /*!
         @brief The resource storage mode used for the CPU mapping for this resource
         */
        [[nodiscard]] StorageMode GetStorageMode() const noexcept
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.GetStorageMode();
        }
        
        /*!
         @brief The size in bytes occupied by this resource
         */
        [[nodiscard]] uint32_t GetAllocatedSize() const noexcept
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.GetAllocatedSize();
        }
        
        /*!
         @brief Set (or query) the purgeability state of a resource
         @discussion Synchronously set the purgeability state of a resource and return what the prior (or current) state is.
         FIXME: If the device is keeping a cached copy of the resource, both the shared copy and cached copy are made purgeable.  Any access to the resource by either the CPU or device will be undefined.
         */
        PurgeableState SetPurgeableState(PurgeableState state)
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.SetPurgeableState(state);
        }
    };
}
        
#define DECLARE_METAL_RESOURCE_INTERFACE \
public:\
[[nodiscard]] CPUCacheMode GetCpuCacheMode() const noexcept;  \
[[nodiscard]] StorageMode GetStorageMode() const noexcept;  \
[[nodiscard]] uint32_t GetAllocatedSize() const noexcept;   \
PurgeableState SetPurgeableState(PurgeableState state);
        
        
#define IMPLEMENT_METAL_RESOURCE_INTERFACE(CppClassName) \
CPUCacheMode CppClassName::GetCpuCacheMode() const noexcept   \
{   \
return ConvertType([(__bridge id<MTLResource>)mNativeHandle cpuCacheMode]);    \
}   \
\
StorageMode CppClassName::GetStorageMode() const noexcept   \
{   \
return ConvertType([(__bridge id<MTLResource>)mNativeHandle storageMode]);    \
}   \
\
uint32_t CppClassName::GetAllocatedSize() const noexcept \
{   \
return [(__bridge id<MTLResource>)mNativeHandle allocatedSize];    \
}   \
\
PurgeableState CppClassName::SetPurgeableState(PurgeableState state) \
{   \
    return ConvertType([(__bridge id<MTLResource>)mNativeHandle setPurgeableState:ConvertType(state)]);    \
}
