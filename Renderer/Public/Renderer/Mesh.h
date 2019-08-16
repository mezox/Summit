#pragma once

#include <Renderer/Renderer.h>
#include <Renderer/Object3d.h>
#include <Renderer/VertexBuffer.h>

#include <memory>

namespace Summit
{
    class Material final
    {
    public:
        TextureId albedoTexture;
    };
    
    class Mesh : public Renderer::Object3d
    {
    public:
        const Renderer::VertexBufferBase* GetVertexBufferPointer() const
        {
            return vertexBuffer.get();
        }
        
    public:
        std::unique_ptr<Renderer::VertexBufferBase> vertexBuffer;
        Material material;
    };
}
