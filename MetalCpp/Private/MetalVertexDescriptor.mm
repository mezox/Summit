#include "pch.h"

#include <MetalCpp/MetalVertexDescriptor.h>
#include "MetalTypeConverter.h"

namespace MTL
{    
    VertexAttributeDescriptor::VertexAttributeDescriptor()
        : NSHandleWrapper((__bridge void*)[[[MTLVertexAttributeDescriptor alloc] init] autorelease])
    {}
    
    VertexAttributeDescriptor::VertexAttributeDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    VertexAttributeDescriptor::VertexAttributeDescriptor(const VertexFormat format, const uint32_t bufferIndex, const uint32_t offset)
        : VertexAttributeDescriptor()
    {
        SetVertexFormat(format);
        SetOffset(offset);
        SetBufferIntex(bufferIndex);
    }
    
    VertexAttributeDescriptor& VertexAttributeDescriptor::operator=(VertexAttributeDescriptor&& other)
    {
        MTLVertexAttributeDescriptor* thisDesc = (__bridge MTLVertexAttributeDescriptor*)mNativeHandle;
        MTLVertexAttributeDescriptor* otherDesc = (__bridge MTLVertexAttributeDescriptor*)other.mNativeHandle;
        
        [thisDesc setFormat:[otherDesc format]];
        [thisDesc setBufferIndex:[otherDesc bufferIndex]];
        [thisDesc setOffset:[otherDesc offset]];
    
        return *this;
    }
    
    void VertexAttributeDescriptor::SetVertexFormat(VertexFormat format) noexcept
    {
        [(__bridge MTLVertexAttributeDescriptor*)mNativeHandle setFormat:ConvertType(format)];
    }
    
    void VertexAttributeDescriptor::SetOffset(uint32_t offset) noexcept
    {
        [(__bridge MTLVertexAttributeDescriptor*)mNativeHandle setOffset:offset];
    }
    
    void VertexAttributeDescriptor::SetBufferIntex(uint32_t bufferIndex) noexcept
    {
        [(__bridge MTLVertexAttributeDescriptor*)mNativeHandle setBufferIndex:bufferIndex];
    }
    
    VertexFormat VertexAttributeDescriptor::GetVertexFormat() const noexcept
    {
        return ConvertType([(__bridge MTLVertexAttributeDescriptor*)mNativeHandle format]);
    }
    
    uint32_t VertexAttributeDescriptor::GetOffset() const noexcept
    {
        return [(__bridge MTLVertexAttributeDescriptor*)mNativeHandle offset];
    }
    
    uint32_t VertexAttributeDescriptor::GetBufferIntex() const noexcept
    {
        return [(__bridge MTLVertexAttributeDescriptor*)mNativeHandle bufferIndex];
    }
    
    
    VertexBufferLayoutDescriptor::VertexBufferLayoutDescriptor()
        : NSHandleWrapper((__bridge void*)[[[MTLVertexBufferLayoutDescriptor alloc] init] autorelease])
    {}
    
    VertexBufferLayoutDescriptor::VertexBufferLayoutDescriptor(void *handle)
        : NSHandleWrapper(handle)
    {}
    
    VertexBufferLayoutDescriptor::VertexBufferLayoutDescriptor(const uint32_t stride, const VertexStepFunction stepFunc, const uint32_t stepRate)
        : VertexBufferLayoutDescriptor()
    {
        SetStride(stride);
        SetVertexStepFunction(stepFunc);
        SetStepRate(stepRate);
    }
    
    VertexBufferLayoutDescriptor& VertexBufferLayoutDescriptor::operator=(VertexBufferLayoutDescriptor&& other)
    {
        MTLVertexBufferLayoutDescriptor* thisDesc = (__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle;
        MTLVertexBufferLayoutDescriptor* otherDesc = (__bridge MTLVertexBufferLayoutDescriptor*)other.mNativeHandle;
        
        [thisDesc setStride:[otherDesc stride]];
        [thisDesc setStepFunction:[otherDesc stepFunction]];
        [thisDesc setStepRate:[otherDesc stepRate]];
        
        return *this;
    }
    
    void VertexBufferLayoutDescriptor::SetStride(const uint32_t stride) noexcept
    {
        return [(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle setStride:stride];
    }
    
    void VertexBufferLayoutDescriptor::SetVertexStepFunction(const VertexStepFunction stepFunc) noexcept
    {
        return [(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle setStepFunction:ConvertType(stepFunc)];
    }
    
    void VertexBufferLayoutDescriptor::SetStepRate(const uint32_t stepRate) noexcept
    {
        return [(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle setStepRate:stepRate];
    }
    
    uint32_t VertexBufferLayoutDescriptor::GetStride() const noexcept
    {
        return [(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle stride];
    }
    
    VertexStepFunction VertexBufferLayoutDescriptor::GetStepFunction() const noexcept
    {
        return ConvertType([(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle stepFunction]);
    }
    
    uint32_t VertexBufferLayoutDescriptor::GetStepRate() const noexcept
    {
        return [(__bridge MTLVertexBufferLayoutDescriptor*)mNativeHandle stepRate];
    }
    
    VertexDescriptor::VertexDescriptor()
        : NSHandleWrapper((__bridge void*)[[[MTLVertexDescriptor alloc] init] autorelease])
    {}
    
    VertexDescriptor::VertexDescriptor(void* handle)
        : NSHandleWrapper(handle)
    {}
    
    MTLArrayWrapper<VertexAttributeDescriptor> VertexDescriptor::GetAttributes() const noexcept
    {
        MTLVertexDescriptor* nativeDesc = (__bridge MTLVertexDescriptor*)mNativeHandle;
        return MTLArrayWrapper<VertexAttributeDescriptor>((__bridge void*)nativeDesc.attributes);
    }
    
    MTLArrayWrapper<VertexBufferLayoutDescriptor> VertexDescriptor::GetLayouts() const noexcept
    {
        MTLVertexDescriptor* nativeDesc = (__bridge MTLVertexDescriptor*)mNativeHandle;
        return MTLArrayWrapper<VertexBufferLayoutDescriptor>((__bridge void*)nativeDesc.layouts);
    }
    
    void VertexDescriptor::Reset() noexcept
    {
        [(__bridge MTLVertexDescriptor*)mNativeHandle reset];
    }
}
