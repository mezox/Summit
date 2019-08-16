//
//  ViewController.m
//  Renderer
//
//  Created by Tomas Kubovcik on 03/06/2019.
//  Copyright © 2019 Summit. All rights reserved.
//

#import "ViewController.h"

@implementation ViewController
{
//    std::unique_ptr<Module> metal;
//    std::unique_ptr<Module> gl;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    metal = std::make_unique<Module>("libMetalRenderer.dylib");
//    gl = std::make_unique<Module>("libOpenGLRenderer.dylib");
//
//    metal->Load();
//
//    const auto createMetalRendererFunc = metal->GetFunctionPointer("createRenderer");
//    FuncParam mf = (FuncParam)createMetalRendererFunc;
//
//    const auto metalRenderer = (Summit::IRenderer*)mf();
//    std::cout << "MetalRenderer device info: " << metalRenderer->GetDeviceDescription() << std::endl;
//
//    //metalRenderer->SetupView((__bridge void*)self.view);
//
//    std::vector<float> vertices =
//    {
//        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f,
//        0.0f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f,
//        1.0f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f
//    };
//
//    Summit::BufferDesc bufferDesc;
//    bufferDesc.usage = Summit::BufferUsage::VertexBuffer;
//    bufferDesc.size = 2 * 3 * sizeof(float) + 3 * 4 * sizeof(float);
//    bufferDesc.data = &vertices;
//
//    //const auto bufferId = metalRenderer->CreateBuffer(bufferDesc);
//    //std::cout << "MetalRenderer: Created buffer with id: " << bufferId.id << std::endl;
//
//    gl->Load();
//
//    const auto createGLRendererFunc = gl->GetFunctionPointer("createRenderer");
//    FuncParam gf = (FuncParam)createGLRendererFunc;
//
//    const auto glRenderer = (Summit::IRenderer*)gf();
//    std::cout << "GLRenderer device info: " << glRenderer->GetDeviceDescription() << std::endl;
//
////    gl->Unload();
////    metal->Unload();
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}
@end
