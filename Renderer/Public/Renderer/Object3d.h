#pragma once

#include "VertexBuffer.h"
#include "Renderer.h"

#include <memory>
#include <optional>
#include <vector>
#include <cstdint>

constexpr size_t VBPositionData = 0;
constexpr size_t VBColorData = 1;
constexpr size_t VBTexCoordData = 2;

namespace Summit::Renderer
{
    struct TextureDescriptor
    {
        TextureId texture;
        uint32_t index{ 0 };
        uint32_t shaderType{ 0 };
    };
    
    struct BufferDescriptor
    {
        BufferId buffer;
        uint32_t index{ 0 };
        uint32_t shaderType{ 0 };
    };
    
    class Object3d
    {
    public:
        virtual const VertexBufferBase* GetVertexBufferPointer() const = 0;
        
    public:
        bool writeDepth{ false };
        bool testDepth{ false };
        std::vector<TextureDescriptor> textures;
        std::vector<BufferDescriptor> buffers;
    };
}
