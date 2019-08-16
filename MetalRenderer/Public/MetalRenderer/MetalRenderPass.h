#pragma once

#include <MetalCpp/MetalCommandEncoder.h>
#include <MetalCpp/MetalRenderPass.h>
#include <MetalCpp/MetalCommandBuffer.h>

namespace Summit
{
    class CommandQueue;
    
    class RenderPass final
    {
    public:
        RenderPass() = default;
        
        void Begin(const MTL::RenderPassDescriptor& desc, const MTL::CommandQueue& queue, const char* name = "");
        void End() noexcept;
        
        bool IsEncodingRenderCommands() const noexcept;
        
        MTL::RenderCommandEncoder& GetRenderCommandEncoder() { return mRenderEncoder; }
        
    private:
        MTL::RenderCommandEncoder mRenderEncoder;
        MTL::CommandBuffer mCommandBuffer;
    };
}
