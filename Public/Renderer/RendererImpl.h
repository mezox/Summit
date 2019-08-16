#pragma once

#include <Renderer/Renderer.h>

#include <vector>

namespace Summit
{
    class Renderer : public IRenderer
    {
    public:
        Renderer();
        virtual ~Renderer();
        
        GraphicsAPI GetAPI() const override;
        std::string GetDeviceDescription() const override;
        
        void SetupView(void* view) override;
        
        BufferId CreateBuffer(const BufferDesc& desc) override;
        
    private:
        class Impl;
        std::unique_ptr<Impl> mWrapper;
    };
}
