#include <Renderer/Layer.h>

namespace Summit
{
    Layer::Layer(std::string debugName)
        : mDebugName(std::move(debugName))
    {}
    
    Layer::~Layer()
    {}
}
