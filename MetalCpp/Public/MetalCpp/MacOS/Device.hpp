#pragma once

#include <MetalCpp/MetalCppModule.h>
#include <MetalCpp/MetalWrapperCommon.h>

namespace MTL
{
    class MetalDevice;
}

namespace MTL::Device
{
    /*!
     @brief On systems that support automatic graphics switching, this will return YES for the the low power device.
     */
    METAL_CPP_API bool IsLowPower(const MetalDevice& device);
    
    /*!
     @brief On systems that include more that one GPU, this will return YES for any device that does not support any displays.
     */
    METAL_CPP_API bool IsHeadless(const MetalDevice& device);
    
    /*!
     @brief Returns all Metal devices in the system.
     @discussion This API will not cause the system to switch devices and leaves the
     decision about which GPU to use up to the application based on whatever criteria
     it deems appropriate.
     */
    METAL_CPP_API MTLArrayWrapper<MetalDevice> CopyAllDevices();
}
