#import <AppKit/NSWindow.h>

namespace Summit::AppKit
{
    class IWindowEvents;
}

@interface SAWindow : NSWindow
@end

@interface SAWindowDelegate : NSObject<NSWindowDelegate>
- (instancetype)initWithWindow:(SAWindow*)window events:(Summit::AppKit::IWindowEvents*)events;
@end
