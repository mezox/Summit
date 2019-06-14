#pragma once

#import <Foundation/Foundation.h>

@interface GLRendererWrapper : NSObject
-(instancetype)initWithName:(NSString*)name;
-(NSString*)getActiveDeviceDescription;
@end
