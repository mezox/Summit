#include "pch.h"

#include <MetalCpp/MetalBuffer.h>
#include "MetalTypeConverter.h"

namespace MTL
{
    Buffer::Buffer(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    void* Buffer::Contents() const noexcept
    {
        return [(__bridge id<MTLBuffer>)mNativeHandle contents];
    }
    
    void Buffer::DidModifyRange(const uint32_t location, const uint32_t length) const noexcept
    {
        [(__bridge id<MTLBuffer>)mNativeHandle didModifyRange:NSMakeRange(location, length)];
    }
    
    IMPLEMENT_METAL_DEVICE_RESOURCE_ID_INTERFACE(Buffer)
    IMPLEMENT_METAL_RESOURCE_INTERFACE(Buffer)
}
