#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalResource.h"
#include "MetalTypes.h"

#include <cstdint>

namespace MTL
{
    /*!
     @abstract MTLTextureUsage declares how the texture will be used over its lifetime (bitwise OR for multiple uses).
     @discussion This information may be used by the driver to make optimization decisions.
     */
    enum class TextureUsage
    {
        Unknown,
        ShaderRead,
        ShaderWrite,
        RenderTarget,
        PixelFormatView
    }; // TODO:: This should be bitmask
    
    class METAL_CPP_API TextureDescriptor : public NSHandleWrapper
    {
        friend class MetalDevice;
        
    public:
        TextureDescriptor(uint32_t width, uint32_t height);
        TextureDescriptor(uint32_t width, uint32_t height, PixelFormat format);
        TextureDescriptor(uint32_t width, uint32_t height, PixelFormat format, TextureUsage usage);
        TextureDescriptor(uint32_t width, uint32_t height, PixelFormat format, TextureUsage usage, StorageMode mode);
        
        void SetPixelFormat(PixelFormat format);
        void SetUsage(TextureUsage usage);
        void SetStorageMode(StorageMode mode);
        void SetMipMapLevelCount(uint32_t levels);
    };
    
    class METAL_CPP_API Texture final : public NSHandleWrapper, public Resource<Texture>
    {
        friend class RenderCommandEncoder;
        friend class RenderPassAttachmentDescriptor;
        
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        DECLARE_METAL_RESOURCE_INTERFACE
        
    public:
        explicit Texture(void* handle);
        
        /*!
         @abstract Convenience for replaceRegion:mipmapLevel:slice:withBytes:bytesPerRow:bytesPerImage: that doesn't require slice related arguments
         */
        void ReplaceRegion(const Region& region, const uint8_t mipmapLevel, const void* pixelBytes, const uint32_t bytesPerRow) noexcept;
    };
}
