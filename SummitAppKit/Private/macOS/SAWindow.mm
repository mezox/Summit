#import "SAWindow.h"

#import <Foundation/NSNotification.h>

#include <SummitAppKit/Window.h>

float transformY(float y)
{
    return CGDisplayBounds(CGMainDisplayID()).size.height - y - 1;
}

@implementation SAWindowDelegate
{
    SAWindow* window;
    Summit::AppKit::IWindowEvents* windowEvents;
}

- (instancetype)initWithWindow:(SAWindow*)window events:(Summit::AppKit::IWindowEvents*)events;
{
    self = [super init];
    
    if (self != nil)
    {
        self->window = window;
        self->windowEvents = events;
    }
    
    return self;
}

- (BOOL)windowShouldClose:(id)sender
{
    return YES;
}

- (BOOL)releasedWhenClosed:(id)sender
{
    return YES;
}

- (void)windowDidResize:(NSNotification *)notification
{
    
}

- (void)windowDidMove:(NSNotification *)notification
{
    @autoreleasepool
    {
        const NSRect contentRect = [window contentRectForFrameRect:[window frame]];
        
        int x = contentRect.origin.x;
        int y = transformY(contentRect.origin.y + contentRect.size.height - 1);
        
        windowEvents->DidMove(x, y);
    }
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
    
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
    
}

- (void)windowWillClose:(NSNotification *)notification
{
    
}
@end

@implementation SAWindow
- (BOOL)canBecomeKeyWindow
{
    // Required for NSWindowStyleMaskBorderless windows
    return YES;
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

@end
