#pragma once

#include "MetalCppModule.h"
#include <CoreFoundation/CFBase.h>

#include <cstdint>

namespace MTL
{
    class METAL_CPP_API NSHandleWrapper
    {        
    public:
        NSHandleWrapper() = default;
        
        explicit NSHandleWrapper(void* handle)
            : mNativeHandle(handle)
        {
            if(mNativeHandle)
            {
                CFRetain(mNativeHandle);
            }
        }
        
        NSHandleWrapper(const NSHandleWrapper& other) = delete;
        
        NSHandleWrapper(NSHandleWrapper&& other) noexcept
            : mNativeHandle(other.mNativeHandle)
        {
            other.mNativeHandle = nullptr;
        }
        
        NSHandleWrapper& operator=(const NSHandleWrapper& other) = delete;
        
        NSHandleWrapper& operator=(NSHandleWrapper&& other) noexcept
        {
            if (other.mNativeHandle == mNativeHandle)
                return *this;
            
            if (mNativeHandle)
                CFRelease(mNativeHandle);
            
            mNativeHandle = other.mNativeHandle;
            other.mNativeHandle = nullptr;
            return *this;
        }
        
        
        virtual ~NSHandleWrapper()
        {
            if(mNativeHandle)
            {
                CFRelease(mNativeHandle);
                mNativeHandle = nullptr;
            }
        }
        
        const void* GetPtr() const noexcept
        {
            return mNativeHandle;
        }
        
        bool IsValid() const noexcept
        {
            return mNativeHandle != nullptr;
        }
        
    public:
        void* mNativeHandle{ nullptr };
    };

    class METAL_CPP_API MTLArrayWrapperBase : private NSHandleWrapper
    {
    public:
        MTLArrayWrapperBase(void* handle);
        
        uint32_t GetSize() const noexcept;
        
    protected:
        void* GetItem(uint32_t index) const noexcept;
    };
    
    template<typename T>
    class MTLArrayWrapper final : public MTLArrayWrapperBase
    {
    public:
        MTLArrayWrapper(void* handle)
            : MTLArrayWrapperBase(handle)
        {}
        
        T operator[](uint32_t index)
        {
            return T{ GetItem(index) };
        }
    };
}
