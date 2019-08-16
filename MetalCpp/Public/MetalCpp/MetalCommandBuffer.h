#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalDeviceResource.h"
#include "MetalCommandEncoder.h"

#include <functional>

namespace MTL
{
//    /class RenderCommandEncoder;
    class RenderPassDescriptor;
    class BlitEncoder;
    class Drawable;
    
    /*!
     @brief Function object to execute on command buffer schedule/complete.
     */
    using CommandBufferHandler = std::function<void(const class CommandBuffer&)>;
    
    /*!
     @brief A serial list of commands for the device to execute.
     */
    class METAL_CPP_API CommandBuffer final : public NSHandleWrapper, public DeviceResource<CommandBuffer>
    {
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        
    public:
        CommandBuffer() = default;
        explicit CommandBuffer(void* handle);
        
        /*!
         @brief If YES, this command buffer holds strong references to objects needed to execute this command buffer.
         */
        [[nodiscard]] bool HoldsRetainedReferences() const noexcept;
        
        /*!
         @brief Commit a command buffer so it can be executed as soon as possible.
         */
        void Commit() const;
        
        /*!
         @brief Append this command buffer to the end of its MTLCommandQueue.
         */
        void Enqueue() const;
        
        /*!
         @brief Add a drawable present that will be invoked when this command buffer has been scheduled for execution.
         @discussion The submission thread will be lock stepped with present call been serviced by window server
         */
        void Present(const Drawable& drawable) const;
        
        /*!
         @brief returns a render command endcoder to encode into this command buffer.
         */
        [[nodiscard]] RenderCommandEncoder CreateRenderCommandEncoder(const RenderPassDescriptor& desc) const noexcept;
        
        /*!
         @brief Synchronously wait for this command buffer to be scheduled.
         */
        void WaitUntilScheduled() const noexcept;
        
        /*!
         @brief Synchronously wait for this command buffer to complete.
         */
        void WaitUntilCompleted() const noexcept;
        
        /*!
         @brief returns a blit command encoder to encode into this command buffer.
         */
        [[nodiscard]] BlitEncoder BlitCommandEncoder() const noexcept;
        
        /*!
         @brief The host time in seconds that GPU starts executing this command buffer. Returns zero if it has not started. This usually can be called in command buffer completion handler.
         */
        [[nodiscard]] double GetGPUStartTime() const noexcept;
        
        /*!
         @brief The host time in seconds that GPU finishes executing this command buffer. Returns zero if CPU has not received completion notification. This usually can be called in command buffer completion handler.
         */
        [[nodiscard]] double GetGPUEndTime() const noexcept;
        
        /*!
         @brief Adds a block to be called when this command buffer has been scheduled for execution.
         */
        void AddScheduledHandler(CommandBufferHandler handler) const noexcept;
        
        /*!
         @brief Add a block to be called when this command buffer has completed execution.
         */
        void AddCompletedHandler(CommandBufferHandler handler) const noexcept;
    };
}
