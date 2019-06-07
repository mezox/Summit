#pragma once

#include <Renderer/Renderer.h>

namespace Summit
{
    class MetalRenderer : public IRenderer
    {
    public:
        MetalRenderer();
        virtual ~MetalRenderer();
        
        GraphicsAPI GetAPI() const override;
        
    private:
        class Impl;
        std::unique_ptr<Impl> mWrapper;
    };
}
