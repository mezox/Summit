#import "MetalRendererWrapper.h"
#import "MetalRenderer-Swift.h"

@implementation MetalRendererWrapper
{
    MetalRenderer* swiftRenderer;
}

-(instancetype)initWithName:(NSString*)name
{
    swiftRenderer = [[MetalRenderer alloc] initWithStr: name];
    return self;
}
@end
