#include <SummitAppKit/Application.h>
#include <SummitAppKit/Gui.h>
#include <SummitAppKit/Window.h>

#include <Renderer/Renderer.h>
#include <Renderer/Buffer.h>
#include <Renderer/Layer.h>
#include <Renderer/Framegraph/Framegraph.hpp>

namespace Summit::AppKit
{
    Application::Application(IRenderer& renderer)
        : mRenderer(renderer)
    {
        mWindow = std::make_unique<Window>("SummitAppKit window", 1280, 720, &renderer);
        mWindow->Center();
        mWindow->SetTitled();
        
        mGui = std::make_unique<Gui>(mWindow.get(), &renderer);

        mRenderPasses.push_back(&mDefaultRenderPass);
    }
    
    void Application::Update()
    {
        auto swapChain = mWindow->GetContentView();
        swapChain->Acquire();
        
        Framegraph::Framegraph fg;

        mRenderer.BeginRenderPass(*mWindow->GetContentView(), "GeometryPass");
        for(auto& layerPtr : mLayerManager)
        {
            layerPtr->OnUpdate();
        }
        mRenderer.EndRenderPass();

        if(!mGui)
            return;
        
        FrameData fd;
        fd.width = 1280;
        fd.height = 720;

        mGui->BeginFrame(fd);
        for(auto& layerPtr : mLayerManager)
        {
            layerPtr->RenderGui();
        }

        mGui->EndFrame();

        swapChain->SwapBuffers();
    }
}
