#include "RendererImpl.h"

#import "RendererWrapper.h"

namespace Summit
{
    class Renderer::Impl
    {
    public:
        Impl()
        {
            impl = [[RendererWrapper alloc] initWithName:@"Renderer"];
        }
        
    public:
        RendererWrapper* impl{ nil };
    };

    Renderer::Renderer()
        : mWrapper(std::make_unique<Renderer::Impl>())
    {}
    
    Renderer::~Renderer()
    {}

    GraphicsAPI Renderer::GetAPI() const
    {
        std::string apiString([[mWrapper->impl getActiveDeviceDescription] UTF8String]);
        
        if(apiString == "Metal")
            return GraphicsAPI::Metal;
        else if(apiString == "OpenGL")
            return GraphicsAPI::OpenGL;
    }
    
    std::string Renderer::GetDeviceDescription() const
    {
        return [[mWrapper->impl getActiveDeviceDescription] UTF8String];
    }
}

extern "C"
{
    Summit::IRenderer* createRenderer()
    {
        return new Summit::Renderer();
    }
}
