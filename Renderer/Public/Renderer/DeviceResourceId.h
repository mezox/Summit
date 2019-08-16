#pragma once

#include <type_traits>
#include <cstdint>

namespace Summit
{
    class DeviceResourceIdImpl;
    
    enum class ResourceType
    {
        Buffer,
        UniformBuffer,
        ArgumentBuffer,
        VertexBuffer,
        VertexBufferStream,
        IndexBuffer,
        Texture,
        Sampler,
        Pipeline
    };
    
    class DeviceResourceId
    {
    public:
        DeviceResourceId() = default;
        
    public:
        enum
        {
            c_max_sizeof = 8,
            c_alignment = alignof(uint32_t)
        };
        
        std::aligned_storage_t<c_max_sizeof> mStorage;
    };
    
    class DeviceResourceIdImpl
    {
    public:
        DeviceResourceIdImpl(ResourceType type, uint32_t identifier)
        : type(type), id(identifier)
        {}
        
        static DeviceResourceId Create(ResourceType type, uint32_t id)
        {
            static_assert(sizeof(DeviceResourceIdImpl) <= 8, "Implementation type has bigger size than storage.");
            static_assert(std::alignment_of<DeviceResourceIdImpl>::value <= 8, "Implementation type has bigger alignment than storage");
            
            DeviceResourceId handle{};
            new (&handle.mStorage) uint32_t(8);
            return handle;
        }
        
        ResourceType type{};
        uint32_t id{ 0 };
    };
}
