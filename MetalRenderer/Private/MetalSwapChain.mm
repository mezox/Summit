#include "MetalSwapChain.h"

#include <MetalRenderer/MetalRenderer.h>
#include <MetalCpp/MetalCommandQueue.h>

#import <AppKit/NSColor.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Summit
{
    SwapChain::SwapChain(const uint16_t width, const uint16_t height, const MTL::MetalDevice& device, const MTL::CommandQueue& queue, TextureId depthTexId, MetalRenderAPI* renderer)
        : mCmdQueue(queue)
        , mRenderer(renderer)
        , mDepthTextureId(std::move(depthTexId))
    {
        id<MTLDevice> nativeDevice = (__bridge id<MTLDevice>)device.mNativeHandle;
        
        NSRect frame = NSMakeRect(0, 0, width, height);
        
        CAMetalLayer* mtlLayer = [CAMetalLayer new];
        mtlLayer.presentsWithTransaction = NO;
        mtlLayer.anchorPoint = CGPointMake(0.5, 0.5);
        mtlLayer.frame = frame;
        mtlLayer.magnificationFilter = kCAFilterNearest;
        mtlLayer.minificationFilter = kCAFilterNearest;
        mtlLayer.framebufferOnly = NO;
        mtlLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
        mtlLayer.drawableSize = CGSizeMake(width, height);
        mtlLayer.backgroundColor = NSColor.blackColor.CGColor;
        
        [mtlLayer setDevice:nativeDevice];
        [mtlLayer removeAllAnimations];
        
        @autoreleasepool
        {
            mView = [[SAView alloc] initWithFrame:frame mouseMoved:(sigslot::signal<const PointerEvent&>*)&mMouseMoused view:this];
            if(mView != nil)
            {
                [mView setLayer:mtlLayer];
            }
        }
    }
    
    SwapChain::~SwapChain()
    {
        [mView release];
        mView = nil;
    }
    
    void SwapChain::Acquire()
    {
        CAMetalLayer* layer = static_cast<CAMetalLayer*>([mView layer]);
        mDrawable = layer ? MTL::Drawable((__bridge void*)[layer nextDrawable]) : MTL::Drawable((__bridge void*)nil);
    }
    
    void SwapChain::SwapBuffers()
    {
//        CAMetalLayer* layer = static_cast<CAMetalLayer*>([mView layer]);
//        mDrawable = layer ? Drawable((__bridge void*)[layer nextDrawable]) : Drawable((__bridge void*)nil);
        
        @autoreleasepool
        {
            const auto presentCmdBuffer = mCmdQueue.CreateCommandBuffer();
            presentCmdBuffer.Present(mDrawable);
            presentCmdBuffer.Commit();
        }
    }
    
    const uint32_t SwapChain::GetWidth() const noexcept
    {
        @autoreleasepool
        {
            const NSRect rect = [mView frame];
            return static_cast<uint32_t>(rect.size.width);
        }
    }
    
    const uint32_t SwapChain::GetHeight() const noexcept
    {
        @autoreleasepool
        {
            const NSRect rect = [mView frame];
            return static_cast<uint32_t>(rect.size.height);
        }
    }
    
    const uint32_t SwapChain::GetFramebufferWidth() const noexcept
    {
        @autoreleasepool
        {
            const NSRect frame = [mView frame];
            const NSRect rect = [mView convertRectToBacking:frame];
            
            return static_cast<uint32_t>(rect.size.width);
        }
    }
    
    const uint32_t SwapChain::GetFramebufferHeight() const noexcept
    {
        @autoreleasepool
        {
            const NSRect frame = [mView frame];
            const NSRect rect = [mView convertRectToBacking:frame];
            
            return static_cast<uint32_t>(rect.size.height);
        }
    }
    
    void* SwapChain::GetNativeHandle() noexcept
    {
        return (__bridge void*)mView;
    }
    
    void SwapChain::Resize(uint32_t width, uint32_t height)
    {
        // wait until device idle
        
        // Resize CAMetalLayer
        CAMetalLayer* mtlLayer = static_cast<CAMetalLayer*>(mView.layer);
        mtlLayer.drawableSize = CGSizeMake(width, height);
        
//        try
//        {
//            TextureId newBackBuffer = mRenderer->CreateTexture2D(width, height);
//            
//            mBackBuffer[1] = newBackBuffer;
//            mBackBuffer[0] = newBackBuffer;
//        }
//        catch (...)
//        {
//            
//        }
    }
    
    const MTL::Drawable& SwapChain::GetDrawable() const noexcept
    {
        return mDrawable;
    }
}
