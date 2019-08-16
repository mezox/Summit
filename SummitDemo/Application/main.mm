//
//  main.m
//  Renderer
//
//  Created by Tomas Kubovcik on 03/06/2019.
//  Copyright © 2019 Summit. All rights reserved.
//

#import <AppKit/NSApplication.h>
#import "AppDelegate.h"

int main(int argc, const char * argv[])
{
    @autoreleasepool
    {
        NSApplication * application = [NSApplication sharedApplication];
        AppDelegate* appDelegate = [[[AppDelegate alloc] init] autorelease];
        [application setDelegate:appDelegate];
        [application run];
    }
    
    return 0;
}
