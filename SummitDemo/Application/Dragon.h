#pragma once

#include <Renderer/Object3d.h>
#include <Renderer/Renderer.h>

#include "ObjLoader.h"
#include "Math.h"

class Dragon final : public Summit::Renderer::Object3d
{
public:
    Dragon(Summit::IRenderer& renderer);
    
    const Summit::Renderer::VertexBufferBase* GetVertexBufferPointer() const override;
    
private:
    Summit::Renderer::VertexBuffer<Vec4, Vec4, Vec4> mVertexBuffer;
};
