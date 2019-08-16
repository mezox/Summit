#pragma once

namespace Summit
{
    struct TextureDescriptor
    {
        uint16_t width{ 0 };
        uint16_t height{ 0 };
        uint8_t mipMapLevels{ 0 };
    };
    
    class Texture
    {
    public:
        TextureId textureId;
    };
}
