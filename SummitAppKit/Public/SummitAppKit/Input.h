#pragma once

#include <SummitAppKit/Window.h>

namespace Summit
{
    template<class T>
    class ServiceLocator
    {
    public:
        static void Provide(T&& service)
        {
            Storage() = std::move(service);
        }

        static T& Service()
        {
            return Storage();
        }

    private:
        static T& Storage()
        {
            static T service;
            return service;
        }
    };

    template<typename D>
    class Input
    {
    public:
        Input() = default;

        bool IsMouseButtonPressed(const AppKit::Window* window) const
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.IsMouseButtonPressedImpl(window);
        }

    private:

    };

    class MacInput final : public Input<MacInput>
    {
    public:
        bool IsMouseButtonPressedImpl(const AppKit::Window* window) const
        {
            return true;
        }
    };

    //#if defined(MACOS)
    #define InputService Input<MacInput>
    #define InputServiceLocator ServiceLocator<InputService>
    //#endif
}
