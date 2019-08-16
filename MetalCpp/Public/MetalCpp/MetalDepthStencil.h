#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
//#include "MetalDeviceResource.h"

namespace MTL
{
    enum class CompareFunction
    {
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always
    };
    
    class METAL_CPP_API DepthStencilDescriptor final : public NSHandleWrapper
    {
        friend class MetalDevice;
        
    public:
        DepthStencilDescriptor();
        explicit DepthStencilDescriptor(void* handle);
        
        /* Defaults to MTLCompareFuncAlways, which effectively skips the depth test */
        void SetDepthCompareFunction(const CompareFunction cmpFunc) const noexcept;
        
        /* Defaults to NO, so no depth writes are performed */
        void SetDepthWriteEnabled(const bool toggle) const noexcept;
    };
    
    class METAL_CPP_API DepthStencilState final : public NSHandleWrapper//, public DeviceResource<DepthStencilState>
    {
        friend class RenderCommandEncoder;
        
        //DECLARE_DEVICE_RESOURCE
        
    public:
        DepthStencilState() = delete;
        explicit DepthStencilState(void* handle);
    };
}
