#pragma once

#include <Renderer/RendererBase.h>

namespace Summit
{
    enum class BufferUsage
    {
        Undefined,
        VertexBuffer,
        IndexBuffer
    };

    struct BufferDesc
    {
        BufferUsage usage;
        uint32_t size{ 0 };
        void* data{ nullptr };
    };
    
    struct Buffer
    {
        
    };
}
