#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"

namespace MTL
{
    class CommandBuffer;
    
    /*!
     @brief A serial queue of command buffers to be executed by the device.
     */
    class METAL_CPP_API CommandQueue final : public NSHandleWrapper//, public DeviceResource<CommandQueue>
    {
    public:
        CommandQueue() = default;
        explicit CommandQueue(void* handle);
        
        /*!
         @brief Returns a new autoreleased command buffer used to encode work into this queue that
         maintains strong references to resources used within the command buffer.
         */
        [[nodiscard]] CommandBuffer CreateCommandBuffer() const noexcept;
        
        /*!
         @brief Returns a new autoreleased command buffer used to encode work into this queue that
         does not maintain strong references to resources used within the command buffer.
         */
        [[nodiscard]] CommandBuffer CommandBufferWithUnretainedReferences() const noexcept;
    };
}
