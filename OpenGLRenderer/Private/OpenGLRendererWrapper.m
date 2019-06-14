#import "OpenGLRendererWrapper.h"
#import "OpenGLRenderer-Swift.h"

@implementation GLRendererWrapper
{
    GLRenderer* swiftRenderer;
}

-(instancetype)initWithName:(NSString*)name
{
    swiftRenderer = [[GLRenderer alloc] initWithStr: name];
    return self;
}

-(NSString*)getActiveDeviceDescription
{
    return [swiftRenderer getActiveDeviceDescription];
}
@end
