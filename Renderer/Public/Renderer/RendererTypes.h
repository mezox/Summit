#pragma once

#include <cstdint>

namespace Summit
{
    struct BufferId     { uint32_t id{ 0 }; };
    struct PipelineId
    {
        uint32_t id{ 0 };
        
        bool operator==(const PipelineId& other) const
        {
            return id == other.id;
        }
    };
    struct TextureId    { uint32_t id{ 0 }; };
    struct SamplerId    { uint32_t id{ 0 }; };
    
    template<typename T>
    struct Rect
    {
        T x{ 0 };
        T y{ 0 };
        T width{ 0 };
        T height{ 0 };
    };
    
    template<typename T>
    struct Viewport
    {
        Rect<T> rect;
        T near{ 0 };
        T far{ 0 };
    };
}
