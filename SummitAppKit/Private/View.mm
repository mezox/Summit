//#include <SummitAppKit/View.h>
//
//#import "macOS/SAView.h"
//#import <QuartzCore/CAMetalLayer.h>
//#import <AppKit/NSColor.h>
//
//namespace Summit::AppKit
//{
//    class View::NativeView
//    {
//    public:
//        NativeView(const uint16_t width, const uint16_t height, void* creator)
//        {
//            if(!creator)
//            {
//                throw std::runtime_error("No creator");
//            }
//            
//            id<MTLDevice> device = (__bridge id<MTLDevice>)creator;
//            
//            view = [[SAView alloc] initWithFrame:NSMakeRect(0, 0, width, height) device:device];
//            if(view != nil)
//            {
//                [view setWantsLayer:YES];
//                [view setFrame:NSMakeRect(0, 0, width, height)];
//                view.layer.backgroundColor = NSColor.blackColor.CGColor;
//            }
//        }
//        
//        ~NativeView()
//        {
//            view = nil;
//        }
//        
//    public:
//        SAView* view = nil;
//    };
//    
//    View::View(uint16_t width, uint16_t height, void* creator)
//        : mNativeView(std::make_unique<NativeView>(width, height, creator))
//    {}
//    
//    View::~View()
//    {}
//    
//    void View::AcquireNextImage()
//    {
//        
//    }
//
//    const uint32_t View::GetWidth() const noexcept
//    {
//        const NSRect rect = [mNativeView->view frame];
//        return static_cast<uint32_t>(rect.size.width);
//    }
//
//    const uint32_t View::GetHeight() const noexcept
//    {
//        const NSRect rect = [mNativeView->view frame];
//        return static_cast<uint32_t>(rect.size.height);
//    }
//    
//    const void* View::GetNativeHandle() const noexcept
//    {
//        return (__bridge void*)mNativeView->view;
//    }
//}
