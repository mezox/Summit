#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalResource.h"

namespace MTL
{
    /*!
     @brief A typeless allocation accessible by both the CPU and the GPU (MTLDevice) or by only the GPU when the storage mode is
     MTLResourceStorageModePrivate.
     
     @discussion
     Unlike in OpenGL and OpenCL, access to buffers is not synchronized.  The caller may use the CPU to modify the data at any time
     but is also responsible for ensuring synchronization and coherency.
     
     The contents become undefined if both the CPU and GPU write to the same buffer without a synchronizing action between those writes.
     This is true even when the regions written do not overlap.
     */
    class METAL_CPP_API Buffer final : public NSHandleWrapper, public Resource<Buffer>
    {
        friend class RenderCommandEncoder;
        friend class BlitEncoder;
        
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        DECLARE_METAL_RESOURCE_INTERFACE
        
    public:
        Buffer() = delete;
        explicit Buffer(void* handle);
        
        Buffer(Buffer&& other) = default;
        Buffer& operator=(Buffer&& other) = default;
        
        /*!
         @brief Returns the data pointer of this buffer's shared copy.
         */
        [[nodiscard]] void* Contents() const noexcept;
        
        /*!
         @brief Inform the device of the range of a buffer that the CPU has modified, allowing the implementation to invalidate
         its caches of the buffer's content.
         @discussion When the application writes to a buffer's sysmem copy via @a contents, that range of the buffer immediately
         becomes undefined for any accesses by the GPU (MTLDevice).  To restore coherency, the buffer modification must be followed
         by -didModifyRange:, and then followed by a commit of the MTLCommandBuffer that will access the buffer.
         -didModifyRange does not make the contents coherent for any previously committed command buffers.
         Note: This method is only required if buffer is created with a storage mode of MTLResourceStorageModeManaged.
         It is not valid to invoke this method on buffers of other storage modes.
         @param range The range of bytes that have been modified.
         */
        void DidModifyRange(const uint32_t location, const uint32_t length) const noexcept;
    };
}
