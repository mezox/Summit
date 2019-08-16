#pragma once

#include <Renderer/CommandBuffer.h>

#include <string>

namespace Summit
{
    namespace Framegraph
    {
        class Framegraph;
    }
    
    class Layer
    {
    public:
        explicit Layer(std::string debugName);
        virtual ~Layer();
        
        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void OnUpdate() = 0;
        virtual void RenderGui() const = 0;
        
    protected:
        std::string mDebugName;
        Summit::CmdList mRenderCmds;
    };
}
