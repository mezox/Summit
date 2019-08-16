#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"

namespace MTL
{
    /*!
     @abstract Options for filtering texels within a mip level.
     */
    enum class SamplerMinMagFilter
    {
        /*!
        @constant Nearest Select the single texel nearest to the sample point.
         */
        Nearest,

        /*!
        @constant Linear Select two texels in each dimension, and interpolate linearly between them.  Not all devices support linear filtering for all formats.  Integer textures can not use linear filtering on any device, and only some devices support linear filtering of Float textures.
         */
        Linear
    };
    
    /*!
     @abstract Options for selecting and filtering between mipmap levels
     */
    enum class SamplerMipFilter
    {
        /*!
         @constant NotMipmapped The texture is sampled as if it only had a single mipmap level.  All samples are read from level 0.
         */
        NotMipmapped,
        
        /*!
         @constant Nearest The nearst mipmap level is selected.
         */
        Nearest,
        
        /*!
        @constant Linear If the filter falls between levels, both levels are sampled, and their results linearly interpolated between levels.
        */
        Linear
    };
//
//    /*!
//     @enum MTLSamplerAddressMode
//     @abstract Options for what value is returned when a fetch falls outside the bounds of a texture.
//
//     @constant MTLSamplerAddressModeClampToEdge
//     Texture coordinates will be clamped between 0 and 1.
//
//     @constant MTLSamplerAddressModeMirrorClampToEdge
//     Mirror the texture while coordinates are within -1..1, and clamp to edge when outside.
//
//     @constant MTLSamplerAddressModeRepeat
//     Wrap to the other side of the texture, effectively ignoring fractional parts of the texture coordinate.
//
//     @constant MTLSamplerAddressModeMirrorRepeat
//     Between -1 and 1 the texture is mirrored across the 0 axis.  The image is repeated outside of that range.
//
//     @constant MTLSamplerAddressModeClampToZero
//     ClampToZero returns transparent zero (0,0,0,0) for images with an alpha channel, and returns opaque zero (0,0,0,1) for images without an alpha channel.
//
//     @constant MTLSamplerAddressModeClampToBorderColor
//     Clamp to border color returns the value specified by the borderColor variable of the MTLSamplerDesc.
//     */
//    typedef NS_ENUM(NSUInteger, MTLSamplerAddressMode) {
//        MTLSamplerAddressModeClampToEdge = 0,
//        MTLSamplerAddressModeMirrorClampToEdge API_AVAILABLE(macos(10.11)) API_UNAVAILABLE(ios) = 1,
//        MTLSamplerAddressModeRepeat = 2,
//        MTLSamplerAddressModeMirrorRepeat = 3,
//        MTLSamplerAddressModeClampToZero = 4,
//        MTLSamplerAddressModeClampToBorderColor API_AVAILABLE(macos(10.12)) API_UNAVAILABLE(ios) = 5,
//    } API_AVAILABLE(macos(10.11), ios(8.0));
    
    class METAL_CPP_API SamplerDescriptor final : public NSHandleWrapper
    {
        friend class MetalDevice;
        
    public:
        /*!
         @param minFilter Filter option for combining texels within a mipmap level the sample footprint is larger than a pixel (minification).
         @param magFilter Filter option for combining texels within a mipmap level the sample footprint is smaller than a pixel (magnification).
         */
        SamplerDescriptor(SamplerMinMagFilter minFilter, SamplerMinMagFilter magFilter);
    };
    
    class METAL_CPP_API SamplerState final : public NSHandleWrapper
    {
        friend class RenderCommandEncoder;
    public:
        SamplerState(void* handle);
    };
}
