#import "MetalRendererWrapper.h"
#import "Summit-Swift.h"

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
