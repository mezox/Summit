#include "pch.h"

#include <MetalCpp/MacOS/Device.hpp>
#include <MetalCpp/MetalDevice.h>

namespace MTL::Device
{
    bool IsLowPower(const MetalDevice& device)
    {
        id<MTLDevice> nativeDevice = (__bridge id<MTLDevice>)device.GetPtr();
        return [nativeDevice isLowPower];
    }
    
    bool IsHeadless(const MetalDevice& device)
    {
        id<MTLDevice> nativeDevice = (__bridge id<MTLDevice>)device.GetPtr();
        return [nativeDevice isHeadless];
    }
    
    MTLArrayWrapper<MetalDevice> CopyAllDevices()
    {
        NSArray<id<MTLDevice>>* systemDevices = MTLCopyAllDevices();
        return MTLArrayWrapper<MetalDevice>((__bridge void*)systemDevices);
    }
}
