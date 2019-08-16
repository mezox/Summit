#import <AppKit/NSView.h>

#include <Renderer/Signal.h>
#include <Renderer/PointerEvent.h>
#include <Renderer/SwapChain.h>

@interface SAView : NSView
- (instancetype)initWithFrame:(NSRect)frameRect mouseMoved:(sigslot::signal<const PointerEvent&>*)lambda view:(Summit::Renderer::ISwapChain*)view;
@end
