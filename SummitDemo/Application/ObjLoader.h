#pragma once

#include <Renderer/Object3d.h>
#include <Renderer/Renderer.h>
#include <Renderer/Math.h>
#include <Renderer/Math/Math.h>

struct Vertex
{
    bool operator==(const Vertex& other) const
    {
        return  position == other.position &&
        color == other.color &&
        texCoord == other.texCoord &&
        normal == other.normal;
    }
    
    Vec4 position;
    Vec4 color;
    Math::Float2 texCoord;
    Vec4 normal;
};

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            auto t = std::tie(vertex.position, vertex.color, vertex.texCoord);
            return std::hash<decltype(t)>()(t);
        }
    };
}
