#include "MetalRenderer.h"

#import "MetalRendererWrapper.h"

namespace Summit
{
    class MetalRenderer::Impl
    {
    public:
        Impl()
        {
            impl = [[MetalRendererWrapper alloc] initWithName:@"MetalRenderer"];
        }
        
    public:
        MetalRendererWrapper* impl{ nil };
    };

    MetalRenderer::MetalRenderer()
        : mWrapper(std::make_unique<MetalRenderer::Impl>())
    {}
    
    MetalRenderer::~MetalRenderer()
    {}

    GraphicsAPI MetalRenderer::GetAPI() const
    {
        return GraphicsAPI::Metal;
    }

    std::unique_ptr<IRenderer> CreateRenderer()
    {
        return std::make_unique<MetalRenderer>();
    }
}

extern "C"
{
    Summit::IRenderer* createRenderer()
    {
        return new Summit::MetalRenderer();
    }
}
