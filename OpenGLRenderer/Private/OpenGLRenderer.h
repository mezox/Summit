#pragma once

#include <Renderer/Renderer.h>

namespace Summit
{
    class GLRenderer : public IRenderer
    {
    public:
        GLRenderer();
        virtual ~GLRenderer();
        
        GraphicsAPI GetAPI() const override;
        std::string GetDeviceDescription() const override;
        
    private:
        class Impl;
        std::unique_ptr<Impl> mWrapper;
    };
}
