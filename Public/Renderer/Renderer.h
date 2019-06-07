#pragma once

#include <memory>

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
        
        virtual GraphicsAPI GetAPI() const = 0;
    };
    
    std::unique_ptr<IRenderer> CreateRenderer();
}
