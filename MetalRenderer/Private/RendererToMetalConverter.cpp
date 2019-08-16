#include "pch.h"

#include "RendererToMetalConverter.h"

#include <Renderer/Buffer.h>

#include <MetalCpp/MetalResource.h>

template<>
auto TypeLinkerImpl<Summit::MemoryType, MTL::StorageMode>::operator()(const from_t& mt) -> to_t
{
    switch(mt)
    {
        case Summit::MemoryType::DeviceLocal:       return MTL::StorageMode::Private;
        case Summit::MemoryType::HostVisible:       return MTL::StorageMode::Shared;
        case Summit::MemoryType::HostCoherent:      return MTL::StorageMode::Managed;
    }
    
    throw std::runtime_error("Unimplemented Renderer -> Metal conversion: MemoryType -> StorageMode");
    
    return MTL::StorageMode::Private;
};
