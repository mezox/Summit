#include "OpenGLRenderer.h"

#include <iostream>

namespace Summit
{
    class GLRenderer::Impl
    {
    public:
        Impl()
        {
        }
        
    public:
    };

    GLRenderer::GLRenderer()
        : mWrapper(std::make_unique<GLRenderer::Impl>())
    {
        std::cout << "Instantiated Cpp GL Renderer" << std::endl;
    }
    
    GLRenderer::~GLRenderer()
    {}

    GraphicsAPI GLRenderer::GetAPI() const
    {
        return GraphicsAPI::OpenGL;
    }
}

extern "C"
{
    Summit::IRenderer* createRenderer()
    {
        return new Summit::GLRenderer();
    }
}
