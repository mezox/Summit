//
//  AppDelegate.m
//  Renderer
//
//  Created by Tomas Kubovcik on 03/06/2019.
//  Copyright © 2019 Summit. All rights reserved.
//

#import "AppDelegate.h"
#import <MetalKit/MTKView.h>

#include <SummitAppKit/Window.h>
#include <SummitAppKit/Application.h>
#include <SummitAppKit/Gui.h>

#include <Renderer/Renderer.h>
#include <Renderer/Camera.h>
#include <Renderer/Buffer.h>

#include <Renderer/Math.h>
#include "Cube.h"
#include "Dragon.h"
#include "Chalet.h"

#include <memory>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <vector>

#include <cmath>

#include <Renderer/Math/Math.h>

class Module
{
public:
    Module(const std::string& name)
    : mName(name)
    {}
    
    void Load()
    {
        if (!handle)
        {
            handle = dlopen(mName.c_str(), RTLD_LAZY | RTLD_LOCAL);
            if(!handle)
            {
                std::cout << "Failed to load module metal renderer" << std::endl;
                return;
            }
            
            std::cout << "Loaded module " << mName << std::endl;
        }
    }
    
    void Unload()
    {
        if(handle)
        {
            if (dlclose(handle))
            {
                std::cout << "Failed to unload module " << mName << std::endl;
                return;
            }
            
            handle = nullptr;
            
            std::cout << "Unloaded module " << mName << std::endl;
        }
    }
    
    void* GetFunctionPointer(const std::string& name)
    {
        if (!handle)
        {
            std::cout << "Failed to load function " << name << " from module \"" << mName << "\". Module not loaded" << std::endl;
            return nullptr;
        }
        
        auto funcPtr = dlsym(handle, name.c_str());
        if (!funcPtr)
        {
            std::cout << "Failed to load function " << name << " from module \"" << mName << "\": " << dlerror() << std::endl;
        }
        
        return funcPtr;
    }
    
public:
    std::string mName;
    void* handle{ nullptr };
};

struct InstanceData
{
    Float4x4 model;
};

typedef uint64_t(*FuncParam)();


class TestApplication : public Summit::AppKit::Application
{
public:
    TestApplication(Summit::IRenderer& renderer)
        : Application(renderer)
    {
        using namespace Summit;

        auto chaletLayer = mLayerManager.Push<Chalet>(renderer);

        mCamera3D.SetPosition(Math::Float3(0.0f, 200.0f, 1000.0f));
        mCamera3D.SetRotation(Math::Float3(0.0f, 0.0f, 0.0f));
        mCamera3D.SetPerspectiveProjection(60.0f, 1280.0f, 720.0f, 0.1f, 100000.0f);

        mWindow->MouseEvent.connect([&camera = mCamera3D, view = mWindow->GetContentView()](const PointerEvent& event) {
            if(event.type == MouseEventType::LeftDrag)
            {
                auto diffX = event.x - view->mousePosX;
                auto diffY = event.y - view->mousePosY;

                camera.RotateX(DegreesToRadians(diffY));
                camera.RotateY(DegreesToRadians(diffX));
            }
        });

        mShadowPass.Update.connect([chaletLayer](){
            chaletLayer->OnShadowPass();
        });

        RegisterPreRenderPass(&mShadowPass);
        
        Summit::TextureDescriptor depthBufferDesc;
        depthBufferDesc.width = 1280;
        depthBufferDesc.height = 720;
        depthBufferDesc.mipMapLevels = 0;
        
//        struct DepthPrePass
//        {
//            Summit::Framegraph::TextureResource* depthTexture;
//        };
//        
//        auto depthPrePass = mFramegraph.AddRenderTask<DepthPrePass>("Depth Pre Pass", [&](DepthPrePass& data, Summit::Framegraph::RenderTaskBuilder& builder){
//            data.depthTexture = builder.CreateResource<Summit::Framegraph::TextureResource>("Z-prepass texture", depthBufferDesc);
//        }, [](const DepthPrePass& data){
//            
//        });
//        
//        const auto& depthPrePassData = depthPrePass->GetData();
//        
//        struct FinalPass
//        {
//            Summit::Framegraph::TextureResource* depthTexture;
//        };
//        
//        mFramegraph.AddRenderTask<FinalPass>("Final pass", [&](FinalPass& data, Summit::Framegraph::RenderTaskBuilder& builder){
//            data.depthTexture = builder.ReadResource(data.depthTexture);
//            data.outputTexture = builder.WriteResource(data.)
//        }, [](const FinalPass& data){
//            
//        });
    }

    void Update() override
    {
        mCamera3D.Update();

        mRenderer.BeginFrame();
        mRenderer.BeginScene(mCamera3D);
        Application::Update();
        mRenderer.EndScene();
        mRenderer.EndFrame();
    }

public:
    Summit::Camera mCamera3D;
    Summit::RenderPass mShadowPass;
};



@implementation AppDelegate
{
    Summit::IRenderer* renderer;
    std::unique_ptr<Module> metal;
    std::unique_ptr<TestApplication> application;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application

    metal = std::make_unique<Module>("libMetalRenderer.dylib");
    
    metal->Load();
    
    const auto createMetalRendererFunc = metal->GetFunctionPointer("createRenderer");
    FuncParam mf = (FuncParam)createMetalRendererFunc;
    
    renderer = (Summit::IRenderer*)mf();
    std::cout << "MetalRenderer device info: " << renderer->GetDeviceDescription() << std::endl;
    
    [NSTimer scheduledTimerWithTimeInterval:0.05 target:self selector:@selector(update) userInfo:nil repeats:YES];
    
    application = std::make_unique<TestApplication>(*renderer);
}

- (void)update
{
    application->Update();
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
    metal->Unload();
}
@end
