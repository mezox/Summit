#import "RendererWrapper.h"

#if defined(SUMMIT_GRAPHICS_API_METAL)
#   define SUMMIT_RENDERER  MetalRenderer
#   import "MetalRenderer-Swift.h"
#elif defined(SUMMIT_GRAPHICS_API_GL)
#   define SUMMIT_RENDERER  GLRenderer
#   import "OpenGLRenderer-Swift.h"
#endif


@implementation RendererWrapper
{
    SUMMIT_RENDERER* swiftRenderer;
}

-(instancetype)initWithName:(NSString*)name
{
    swiftRenderer = [[SUMMIT_RENDERER alloc] initWithStr: name];
    return self;
}

-(NSString*)getGraphicsAPI
{
    return [swiftRenderer getGraphicsAPI];
}

-(NSString*)getActiveDeviceDescription
{
    return [swiftRenderer getActiveDeviceDescription];
}
@end
