#pragma once

#include <Renderer/SwapChain.h>
#include <Renderer/Renderer.h>

#include <MetalCpp/MetalCommandBuffer.h>
#include <MetalCpp/MetalDevice.h>
#include <MetalCpp/MetalCommandQueue.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalDrawable.h>

#import <QuartzCore/CAMetalLayer.h>
#import <MacOS/SAView.h>

namespace Summit
{
    class MetalRenderAPI;
    
    class SwapChain final : public Renderer::ISwapChain
    {
    public:
        // TODO: Add pixel format as input param
        SwapChain(uint16_t width, uint16_t height, const MTL::MetalDevice& device, const MTL::CommandQueue& queue, TextureId depthTexId, MetalRenderAPI* renderer);
        ~SwapChain();
        
        void Acquire() override;
        
        const uint32_t GetWidth() const noexcept override;
        const uint32_t GetHeight() const noexcept override;
        const uint32_t GetFramebufferWidth() const noexcept override;
        const uint32_t GetFramebufferHeight() const noexcept override;
        
        void* GetNativeHandle() noexcept override;
        
        TextureId GetDepthAttachmentId() const noexcept { return mDepthTextureId; }
        
        void SwapBuffers() override;
        
        void Resize(uint32_t width, uint32_t height);
        
        [[nodiscard]] const MTL::Drawable& GetDrawable() const noexcept;
        
    private:
        mutable MTL::Drawable mDrawable;
        
        const MTL::CommandQueue& mCmdQueue;
        SAView* mView{ nil };
        MetalRenderAPI* mRenderer{ nullptr };
        TextureId mDepthTextureId;
        TextureId mBackBuffer[2];
    };
}
