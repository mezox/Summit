#pragma once

#import <Foundation/Foundation.h>

@interface RendererWrapper : NSObject
-(instancetype)initWithName:(NSString*)name;
-(NSString*)getGraphicsAPI;
-(NSString*)getActiveDeviceDescription;
@end
