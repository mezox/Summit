#pragma once

#include <Renderer/Renderer.h>

namespace Summit
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        virtual ~Renderer();
        
        GraphicsAPI GetAPI() const override;
        std::string GetDeviceDescription() const override;
        
    private:
        class Impl;
        std::unique_ptr<Impl> mWrapper;
    };
}
