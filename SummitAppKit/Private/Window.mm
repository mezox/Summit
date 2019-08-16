#include <SummitAppKit/Window.h>
#include <Renderer/Renderer.h>
#include <Renderer/SwapChain.h>

#import <Foundation/NSGeometry.h>
#import <AppKit/NSScreen.h>
#import "macOS/SAWindow.h"

#include <iostream>

float TransformY(float y)
{
    auto displayBounds = CGDisplayBounds(CGMainDisplayID());
    return displayBounds.size.height - y - 1;
};

namespace Summit::AppKit
{
    class Window::NativeWindow
    {
    public:
        NativeWindow(Window* wrapper, const std::string& title, const uint16_t width, const uint16_t height)
        {
            NSRect contentRect = NSMakeRect(0, 0, width, height);
            window = [[SAWindow alloc] initWithContentRect:contentRect
                                                 styleMask:NSWindowStyleMaskResizable | NSWindowStyleMaskClosable | NSWindowStyleMaskBorderless
                                                   backing:NSBackingStoreBuffered
                                                     defer:NO];
            
            windowDelegate = [[SAWindowDelegate alloc] initWithWindow:window events:wrapper];
            [window setTitle:[NSString stringWithUTF8String:title.c_str()]];
            [window setDelegate:windowDelegate];
            [window setAcceptsMouseMovedEvents: YES];
            [window orderFront:nil];
        }
        
        ~NativeWindow()
        {
            @autoreleasepool
            {
                [window orderOut:nil];
                [window setContentView:nil];
                [window release];
                
                [windowDelegate release];
            }
            
            window = nil;
            windowDelegate = nil;
        }
        
    public:
        SAWindow* window = nil;
        SAWindowDelegate* windowDelegate = nil;
    };
    
    Window::Window(const std::string& title, const uint16_t width, const uint16_t height, IRenderer* renderer)
        : mNativeWindow(std::make_unique<Window::NativeWindow>(this, title, width, height))
        , mView(renderer->CreateSwapChain(width, height))
    {
        if(mView)
        {
            [mNativeWindow->window setContentView:(__bridge NSView*)mView->GetNativeHandle()];
            
            mView->mMouseMoused.connect([this](const PointerEvent& event){
                MouseEvent(event);
            });
        }
    }
    
    Window::~Window()
    {        
        @autoreleasepool
        {
            if(mView)
            {
                [mNativeWindow->window setContentView:nil];
            }
        }
    }
    
    Renderer::ISwapChain* Window::GetContentView() const noexcept
    {
        return mView.get();
    }
    
    void Window::SetTitle(const std::string& title)
    {
        [mNativeWindow->window setTitle:[NSString stringWithCString:title.c_str() encoding:NSASCIIStringEncoding]];
    }
    
    void Window::Show()
    {
        [mNativeWindow->window makeKeyAndOrderFront:nil];
    }
    
    void Window::Center()
    {
        [mNativeWindow->window center];
    }
    
    void Window::SetTitled()
    {
        [mNativeWindow->window setStyleMask:(NSWindowStyleMaskTitled)];
    }
    
    void Window::SetPosition(const Point2i& screenPosition)
    {
        @autoreleasepool
        {
            const NSRect contentRect = [mNativeWindow->window frame];            
            const NSRect dummyRect = NSMakeRect(screenPosition.x, TransformY(screenPosition.y + contentRect.size.height - 1), 0, 0);
            const NSRect frameRect = [mNativeWindow->window frameRectForContentRect:dummyRect];
            [mNativeWindow->window setFrameOrigin:frameRect.origin];
        }
    }
    
    std::string Window::GetTitle() const noexcept
    {
        @autoreleasepool
        {
            return [[mNativeWindow->window title] cStringUsingEncoding:NSASCIIStringEncoding];
        }
    }
    
    Point2i Window::GetPosition() const noexcept
    {
        @autoreleasepool
        {
            const NSRect frame = [mNativeWindow->window frame];
            const NSRect contentRect = [mNativeWindow->window contentRectForFrameRect:frame];
            
            Point2i position;
            position.x = static_cast<int32_t>(contentRect.origin.x);
            position.y = static_cast<int32_t>(TransformY(contentRect.origin.y + contentRect.size.height - 1));
            
            return position;
        }
    }
    
    void Window::SetSize(uint32_t x, uint32_t y)
    {
        @autoreleasepool
        {
            auto windowFrame = [mNativeWindow->window frame];
            [mNativeWindow->window setFrame:CGRectMake(windowFrame.origin.x, windowFrame.origin.y, x, y) display:YES];
        }
    }
    
    void Window::GetSize(uint32_t& x, uint32_t& y) const noexcept
    {
        @autoreleasepool
        {
            auto windowFrame = [mNativeWindow->window frame];
            x = windowFrame.size.width;
            y = windowFrame.size.height;
        }
    }
    
    void Window::GetCursorPosition(int32_t &x, int32_t &y) const noexcept
    {
        @autoreleasepool
        {
            NSView* view = (__bridge NSView*)mView->GetNativeHandle();
            const NSRect contentRect = [view frame];
            
            // NOTE: The returned location uses base 0,1 not 0,0
            const NSPoint pos = [mNativeWindow->window mouseLocationOutsideOfEventStream];
            x = pos.x;
            y = contentRect.size.height - pos.y;
            
        }
    }
    
    bool Window::IsFocused() const noexcept
    {
        @autoreleasepool
        {
            return [mNativeWindow->window isKeyWindow];
        }
    }
}
