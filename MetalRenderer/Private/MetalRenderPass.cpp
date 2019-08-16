#include <MetalRenderer/MetalRenderPass.h>
#include <MetalCpp/MetalCommandQueue.h>

#include <utility>
#include <iostream>

namespace Summit
{
    bool RenderPass::IsEncodingRenderCommands() const noexcept
    {
        return mRenderEncoder.IsValid();
    }
    
    void RenderPass::Begin(const MTL::RenderPassDescriptor& desc, const MTL::CommandQueue& queue, const char* name)
    {
//        ASSERT(desc.IsValid());
//        ASSERT(!mRenderPassDescriptor.IsValid());
//        ASSERT(!mRenderEncoder.IsValid());
//        ASSERT(!mCommandBuffer.IsValid());
        
        std::string cmdBuffName(name);
        cmdBuffName += "CommandBuffer";
        
        mCommandBuffer = queue.CreateCommandBuffer();
        //ASSERT(mCommandBuffer.IsValid());
        
        mCommandBuffer.AddCompletedHandler([](const MTL::CommandBuffer& cmdBuf){
            
            const auto startTimeGPU = cmdBuf.GetGPUStartTime();
            const auto endTimeGPU = cmdBuf.GetGPUEndTime();
            
            //std::cout << "Draw time GPU: " << endTimeGPU - startTimeGPU << std::endl;
        });
        
        mRenderEncoder = mCommandBuffer.CreateRenderCommandEncoder(desc);
        //ASSERT(mRenderEncoder.IsValid());
        
        mCommandBuffer.SetLabel(cmdBuffName.c_str());
        mRenderEncoder.PushDebugGroup(name);
    }
    
    void RenderPass::End() noexcept
    {
        mRenderEncoder.PopDebugGroup();
        
        if(IsEncodingRenderCommands())
        {
            mRenderEncoder.EndEncoding();
        }
        
        mCommandBuffer.Commit();
    }
}
