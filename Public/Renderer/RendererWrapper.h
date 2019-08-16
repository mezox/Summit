#pragma once

#import <Foundation/Foundation.h>
#import <AppKit/NSView.h>

@interface RendererWrapper : NSObject
-(instancetype)initWithName:(NSString*)name;
-(void)setupWithView:(NSView*)view;
-(void)createBufferWithSize:(NSUInteger)size data:(void*)data;
-(NSString*)getGraphicsAPI;
-(NSString*)getActiveDeviceDescription;
@end
