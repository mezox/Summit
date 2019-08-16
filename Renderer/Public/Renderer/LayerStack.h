#pragma once

#include <Renderer/Layer.h>

#include <vector>
#include <memory>
#include <type_traits>

namespace Summit
{
    class LayerManager final
    {
    public:
        using container_type = std::vector<std::unique_ptr<Layer>>;
        
    public:
        LayerManager();
        ~LayerManager();
        
        template<typename T, typename... Args>
        T* Push(Args&&... args)
        {
            static_assert(std::is_base_of_v<Layer, T>, "Supplied class is not a layer derived class");
            auto it = mLayers.insert(mLayers.begin() + mInsertIdx++, std::make_unique<T>(std::forward<Args>(args)...));
            (*it)->OnAttach();
            
            return static_cast<T*>(it->get());
        }
        
        template<typename T, typename... Args>
        T* PushOverlay(Args&&... args)
        {
            static_assert(std::is_base_of_v<Layer, T>, "Supplied class is not a layer derived class");
            mLayers.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            mLayers.back()->OnAttach();
            
            return static_cast<T*>(mLayers.back().get());
        }
        
        container_type::iterator begin() { return mLayers.begin(); }
        container_type::iterator end() { return mLayers.end(); }

        bool HasLayers() const { return !mLayers.empty(); }
        
    private:
        container_type mLayers;
        unsigned int mInsertIdx{ 0 };
    };
}
