#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include <Renderer/Signal.h>

struct PointerEvent;

template<typename T>
class Point2
{
public:
    constexpr Point2() = default;
    
    Point2(T x, T y) : x(x), y(y) {}
    
    template<typename U>
    Point2(U x, U y)
        : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
    
    T x{};
    T y{};
};

using Point2i = Point2<int32_t>;
using Point2f = Point2<float>;
using Point2d = Point2<double>;

namespace Summit
{
    class IRenderer;
    
    namespace Renderer
    {
        class ISwapChain;
    }
}

namespace Summit::AppKit
{
    class IWindowEvents
    {
    public:
        virtual ~IWindowEvents() = default;
        
    public:
        sigslot::signal<const uint32_t, const uint32_t> Resize;
        sigslot::signal<const uint32_t, const uint32_t> DidMove;
        sigslot::signal<const PointerEvent&> MouseEvent;
    };
    
    class Window final : public IWindowEvents
    {
        class NativeWindow;
        
    public:
        Window(const std::string& title, const uint16_t width, const uint16_t height, IRenderer* renderer);
        ~Window();

        Renderer::ISwapChain* GetContentView() const noexcept;
        
        void SetTitle(const std::string& title);
        void Show();
        void Center();
        void SetTitled();
        
        void SetPosition(const Point2i& screenPosition);
        Point2i GetPosition() const noexcept;
        
        void SetSize(uint32_t x, uint32_t y);
        void GetSize(uint32_t& x, uint32_t& y) const noexcept;
        std::string GetTitle() const noexcept;
        
        void GetCursorPosition(int32_t& x, int32_t& y) const noexcept;
        
        bool IsFocused() const noexcept;
        
    private:
        std::unique_ptr<NativeWindow> mNativeWindow;
        std::unique_ptr<Renderer::ISwapChain> mView;
    };
}
