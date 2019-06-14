#include "OpenGLRenderer.h"
#import "OpenGLRendererWrapper.h"

namespace Summit
{
    class GLRenderer::Impl
    {
    public:
        Impl()
        {
            impl = [[GLRendererWrapper alloc] initWithName:@"GLRenderer"];
        }
        
    public:
        GLRendererWrapper* impl{ nil };
    };

    GLRenderer::GLRenderer()
        : mWrapper(std::make_unique<GLRenderer::Impl>())
    {}
    
    GLRenderer::~GLRenderer()
    {}

    GraphicsAPI GLRenderer::GetAPI() const
    {
        return GraphicsAPI::OpenGL;
    }
    
    std::string GLRenderer::GetDeviceDescription() const
    {
        return [[mWrapper->impl getActiveDeviceDescription] UTF8String];
    }
}

extern "C"
{
    Summit::IRenderer* createRenderer()
    {
        return new Summit::GLRenderer();
    }
}
