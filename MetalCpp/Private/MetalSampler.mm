#include "pch.h"

#include <MetalCpp/MetalSampler.h>

#include "MetalTypeConverter.h"

namespace MTL
{
    SamplerDescriptor::SamplerDescriptor(SamplerMinMagFilter minFilter, SamplerMinMagFilter magFilter)
    {
        auto nativeSamplerDesc = [[MTLSamplerDescriptor alloc] init];
        [nativeSamplerDesc setMinFilter:ConvertType(minFilter)];
        [nativeSamplerDesc setMinFilter:ConvertType(magFilter)];
        [nativeSamplerDesc setMipFilter:MTLSamplerMipFilterLinear];
        [nativeSamplerDesc setSupportArgumentBuffers:YES];
        
        mNativeHandle = (__bridge void*)nativeSamplerDesc;
    }
    
    SamplerState::SamplerState(void* handle)
        : NSHandleWrapper(handle)
    {}
}
