#pragma once

#include "MetalDevice.h"

namespace MTL
{
    template<typename D>
    class DeviceResource
    {
    public:
        DeviceResource() = default;
        
        /*!
         @brief A string to help identify this object.
         */
        [[nodiscard]] const char* GetLabel() const noexcept
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.GetLabel();
        }
        
        /*!
         @brief The device this resource was created against.  This resource can only be used with this device.
         */
        [[nodiscard]] MetalDevice GetDevice() const noexcept
        {
            auto& derived = static_cast<const D&>(*this);
            return derived.GetDevice();
        }
    };
}

#define DECLARE_METAL_DEVICE_RESOURCE_INTERFACE \
public:\
[[nodiscard]] const char* GetLabel() const noexcept;  \
void SetLabel(const char* label) const noexcept;  \
[[nodiscard]] MetalDevice GetDevice() const noexcept;

#define IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(CppClassName) \
void CppClassName::SetLabel(const char* label) const noexcept   \
{   \
    [(__bridge id<MTL##CppClassName>)mNativeHandle setLabel:[NSString stringWithCString:label encoding:NSASCIIStringEncoding]];    \
}   \
\
const char* CppClassName::GetLabel() const noexcept   \
{   \
    return [[(__bridge id<MTL##CppClassName>)mNativeHandle label] cStringUsingEncoding:NSASCIIStringEncoding];    \
}   \
\
MetalDevice CppClassName::GetDevice() const noexcept \
{   \
    id<MTLDevice> device = [(__bridge id<MTL##CppClassName>)mNativeHandle device];    \
    return MetalDevice((__bridge void*)device);  \
}

#define IMPLEMENT_METAL_DEVICE_RESOURCE_INTERFACE(CppClassName) \
void CppClassName::SetLabel(const char* label) const noexcept   \
{   \
    [(__bridge MTL##CppClassName*)mNativeHandle setLabel:[NSString stringWithCString:label encoding:NSASCIIStringEncoding]];    \
}   \
\
const char* CppClassName::GetLabel() const noexcept   \
{   \
    return [[(__bridge MTL##CppClassName*)mNativeHandle label] cStringUsingEncoding:NSASCIIStringEncoding];    \
}   \
\
MetalDevice CppClassName::GetDevice() const noexcept \
{   \
    id<MTLDevice> device = [(__bridge MTL##CppClassName*)mNativeHandle device];    \
    return MetalDevice((__bridge void*)device);  \
}
