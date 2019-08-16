#include "pch.h"

#include <MetalCpp/MetalDepthStencil.h>
#include "MetalTypeConverter.h"

namespace MTL
{
    DepthStencilDescriptor::DepthStencilDescriptor()
        : NSHandleWrapper((__bridge void*)[[[MTLDepthStencilDescriptor alloc] init] autorelease])
    {}
    
    DepthStencilDescriptor::DepthStencilDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    void DepthStencilDescriptor::SetDepthCompareFunction(const CompareFunction cmpFunc) const noexcept
    {
        MTLDepthStencilDescriptor* nativeDescriptor = (__bridge MTLDepthStencilDescriptor*)mNativeHandle;
        [nativeDescriptor setDepthCompareFunction:ConvertType(cmpFunc)];
    }
    
    /* Defaults to NO, so no depth writes are performed */
    void DepthStencilDescriptor::SetDepthWriteEnabled(const bool toggle) const noexcept
    {
        MTLDepthStencilDescriptor* nativeDescriptor = (__bridge MTLDepthStencilDescriptor*)mNativeHandle;
        [nativeDescriptor setDepthWriteEnabled:toggle];
    }
    
    DepthStencilState::DepthStencilState(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    //DEFINE_DEVICE_RESOURCE(DepthStencilDescriptor)
}
