#pragma once

#include <memory>
#include <string>

namespace Summit
{
    enum class GraphicsAPI
    {
        Metal,
        Vulkan,
        OpenGL,
        DirectX
    };
    
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        
        /*!
         @brief Returns enumerated type of underlying graphics API.
         */
        [[nodiscard]] virtual GraphicsAPI GetAPI() const = 0;
        
        /*!
         @brief Returns string description of active device.
         */
        [[nodiscard]] virtual std::string GetDeviceDescription() const = 0;
    };
}
