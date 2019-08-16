#pragma once

#include <cstdint>

namespace Summit
{
    enum class BufferUsage
    {
        Undefined,
        VertexBuffer,
        IndexBuffer,
        UniformBuffer
    };
    
    /*!
     @brief Mask of memory property flags.
     */
    enum class MemoryType
    {
        DeviceLocal,
        HostVisible,
        HostCoherent
    };

    struct BufferDesc
    {
        BufferUsage usage;
        uint32_t size{ 0 };
        void* data{ nullptr };
        MemoryType memoryType{ MemoryType::DeviceLocal };
    };
    
    template<typename T>
    BufferDesc CreateUniformBufferDesc(const T* data)
    {
        BufferDesc desc;
        desc.usage = BufferUsage::UniformBuffer;
        desc.memoryType = MemoryType::HostVisible;
        desc.size = sizeof(T);
        desc.data = (void*)data;
        
        return desc;
    }
    
    struct Buffer
    {
        
    };
}
