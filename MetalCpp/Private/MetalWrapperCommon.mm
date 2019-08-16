#include "pch.h"

#include <MetalCpp/MetalWrapperCommon.h>

namespace MTL
{
    MTLArrayWrapperBase::MTLArrayWrapperBase(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    uint32_t MTLArrayWrapperBase::GetSize() const noexcept
    {
        return static_cast<uint32_t>([(__bridge NSArray*)GetPtr() count]);
    }

    void* MTLArrayWrapperBase::GetItem(uint32_t index) const noexcept
    {
        return (__bridge void*)[(__bridge NSArray*)mNativeHandle objectAtIndexedSubscript:index];
    }
}
