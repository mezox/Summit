#include "pch.h"

#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalTypes.h>

#include "MetalTypeConverter.h"

namespace MTL
{
    TextureDescriptor::TextureDescriptor(const uint32_t width, const uint32_t height)
        : NSHandleWrapper((__bridge void*)[[[MTLTextureDescriptor alloc] init] autorelease])
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setWidth:width];
        [textureDesc setHeight:height];
    }
    
    TextureDescriptor::TextureDescriptor(const uint32_t width, const uint32_t height, PixelFormat format)
        : TextureDescriptor(width, height)
    {
        MTLTextureDescriptor* textureDesc = [[[MTLTextureDescriptor alloc] init] autorelease];
        [textureDesc setPixelFormat:ConvertType(format)];
    }
    
    TextureDescriptor::TextureDescriptor(uint32_t width, uint32_t height, PixelFormat format, TextureUsage usage)
        : TextureDescriptor(width, height, format)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setUsage:ConvertType(usage)];
    }
    
    TextureDescriptor::TextureDescriptor(uint32_t width, uint32_t height, PixelFormat format, TextureUsage usage, StorageMode mode)
    : TextureDescriptor(width, height, format, usage)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setStorageMode:ConvertType(mode)];
    }
    
    void TextureDescriptor::SetPixelFormat(PixelFormat format)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setPixelFormat:ConvertType(format)];
    }
    
    void TextureDescriptor::SetUsage(TextureUsage usage)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setUsage:ConvertType(usage)];
    }
    
    void TextureDescriptor::SetStorageMode(StorageMode mode)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setStorageMode:ConvertType(mode)];
    }
    
    void TextureDescriptor::SetMipMapLevelCount(uint32_t levels)
    {
        MTLTextureDescriptor* textureDesc = (__bridge MTLTextureDescriptor*)mNativeHandle;
        [textureDesc setMipmapLevelCount:levels];
    }
    
    Texture::Texture(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    void Texture::ReplaceRegion(const Region& region, const uint8_t mipmapLevel, const void* pixelBytes, const uint32_t bytesPerRow) noexcept
    {
        auto nativeTexture = (__bridge id<MTLTexture>)mNativeHandle;
        [nativeTexture replaceRegion:MTLRegionMake2D(region.origin.x, region.origin.y, region.size.width, region.size.height)
                         mipmapLevel:0
                           withBytes:pixelBytes
                         bytesPerRow:bytesPerRow];
    }
    
    IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(Texture)
    IMPLEMENT_METAL_RESOURCE_INTERFACE(Texture)
}
