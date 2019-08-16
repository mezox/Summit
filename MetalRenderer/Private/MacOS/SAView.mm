#import "SAView.h"

#import <AppKit/NSEvent.h>
#import <AppKit/NSTrackingArea.h>

#include <Renderer/SwapChain.h>

@implementation SAView
{
    NSTrackingArea* trackingArea;
    sigslot::signal<const PointerEvent&>* mouseMoved;
    Summit::Renderer::ISwapChain* view;
}

- (instancetype)initWithFrame:(NSRect)frameRect
                   mouseMoved:(sigslot::signal<const PointerEvent&>*)lambda
                   view:(Summit::Renderer::ISwapChain*)view
{
    self = [super initWithFrame:frameRect];
    
    if (self != nil)
    {
        [self setPostsFrameChangedNotifications:YES];
        
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(frameDidChangeNotification:)
                                                     name:NSViewFrameDidChangeNotification
                                                   object:self];
        trackingArea = nil;
        [self updateTrackingAreas];
        
        self->mouseMoved = lambda;
        self->view = view;
    }
    
    return self;
}

- (void)frameDidChangeNotification:(NSNotification *)notification
{
}

- (void)updateTrackingAreas
{
    if (trackingArea != nil)
    {
        [self removeTrackingArea:trackingArea];
        [trackingArea release];
    }
    
    const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited |
    NSTrackingActiveInKeyWindow |
    NSTrackingEnabledDuringMouseDrag |
    NSTrackingMouseMoved |
    NSTrackingCursorUpdate |
    NSTrackingInVisibleRect |
    NSTrackingAssumeInside;
    
    NSRect bounds = [self bounds];
    trackingArea = [[NSTrackingArea alloc] initWithRect:bounds
                                                options:options
                                                  owner:self
                                               userInfo:nil];
    
    [self addTrackingArea:trackingArea];
    [super updateTrackingAreas];
}

- (void)mouseEntered:(NSEvent *)event
{
}

- (void)mouseDown:(NSEvent *)event
{
    CGPoint point = [event locationInWindow];
    
    uint16_t x = point.x;
    uint16_t y = self.frame.size.height - point.y;

    view->mMouseButtons[0] = 1;
    
    PointerEvent e;
    e.type = MouseEventType::LeftPress;
    e.x = x;
    e.y = y;
    
    (view->mMouseMoused)(e);
}

- (void)mouseUp:(NSEvent *)event
{
    CGPoint point = [event locationInWindow];
    
    uint16_t x = point.x;
    uint16_t y = self.frame.size.height - point.y;
    
    PointerEvent e;
    e.type = MouseEventType::LeftRelease;
    e.x = x;
    e.y = y;

    view->mMouseButtons[0] = 0;
    
    (*mouseMoved)(e);
}

- (void)mouseMoved:(NSEvent *)event
{
    CGPoint point = [event locationInWindow];
    
    float x = point.x;
    float y = self.frame.size.height - point.y;
    
    PointerEvent e;
    e.type = MouseEventType::Move;
    e.x = x;
    e.y = y;
    
    (*mouseMoved)(e);

    view->mousePosX = x;
    view->mousePosY = y;
}

- (void)mouseDragged:(NSEvent *)event
{
    CGPoint point = [event locationInWindow];

    float x = point.x;
    float y = self.frame.size.height - point.y;

    PointerEvent e;
    e.type = MouseEventType::LeftDrag;
    e.x = x;
    e.y = y;

    (*mouseMoved)(e);

    view->mousePosX = x;
    view->mousePosY = y;
}

- (void)rightMouseDown:(NSEvent *)event
{
}

- (void)rightMouseDragged:(NSEvent *)event
{
    
}

- (void)rightMouseUp:(NSEvent *)event
{
}

- (void)keyDown:(NSEvent *)event
{
}

- (void)scrollWheel:(NSEvent *)event
{
}
@end
