//#include "DeviceResourceIdImpl.h"
//
//#include <type_traits>
//
//namespace Summit
//{
//    DeviceResourceId DeviceResourceIdImpl::Create(const ResourceType type, const uint32_t id)
//    {
//        static_assert(sizeof(DeviceResourceIdImpl) <= 8, "Implementation type has bigger size than storage.");
//        static_assert(std::alignment_of<DeviceResourceIdImpl>::value <= 8, "Implementation type has bigger alignment than storage");
//        
//        DeviceResourceId handle{};
//        new (&handle.mStorage) uint32_t(8);
//        return handle;
//    }
//}
