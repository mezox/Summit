#pragma once

#include <cstdint>

#include <Renderer/Signal.h>
#include <Renderer/PointerEvent.h>

namespace Summit::Renderer
{
    class ISwapChain
    {
    public:
        virtual ~ISwapChain() = default;
        
        virtual void Acquire() = 0;
        
        [[nodiscard]] virtual const uint32_t GetWidth() const noexcept = 0;
        [[nodiscard]] virtual const uint32_t GetHeight() const noexcept = 0;
        [[nodiscard]] virtual const uint32_t GetFramebufferWidth() const noexcept = 0;
        [[nodiscard]] virtual const uint32_t GetFramebufferHeight() const noexcept = 0;
        
        [[nodiscard]] virtual void* GetNativeHandle() noexcept = 0;
        
        virtual void SwapBuffers() = 0;
        
    public:
        sigslot::signal<const PointerEvent&> mMouseMoused;

    public:
        uint8_t mMouseButtons[2]{ 0, 0 };
        double mousePosX{ 0.0 };
        double mousePosY{ 0.0 };
        double oldMousePosX{ 0.0 };
        double oldMousePosY{ 0.0 };
    };
}
