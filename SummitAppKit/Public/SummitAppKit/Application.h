#pragma once

#include <Renderer/LayerStack.h>
#include <Renderer/Camera.h>
#include <Renderer/RenderPass.h>
#include <Renderer/Renderer.h>
#include <Renderer/Framegraph/Framegraph.hpp>
#include <Renderer/Framegraph/Resource.hpp>

#include <deque>

namespace Summit
{
    class IRenderer;
    class Gui;
    
    namespace Framegraph
    {
        using TextureResource = Resource<TextureDescriptor, Texture>;
    }
}

namespace Summit::AppKit
{
    class Window;
    
    class Application
    {
    public:
        Application(IRenderer& renderer);
        virtual ~Application() = default;
        
        virtual void Update();

        template<typename T, typename... Args>
        T* PushLayer(Args&&... args)
        {
            auto layer = mLayerManager.Push<T>(std::forward<Args>(args)...);

            mDefaultRenderPass.Update.connect([layer](){
                layer->OnUpdate();
            });

            return layer;
        }

        void RegisterPreRenderPass(RenderPass* pass) { mRenderPasses.push_front(pass); }
        void RegisterPostRenderPass(RenderPass* pass) { mRenderPasses.push_back(pass); }
        
    protected:
        IRenderer& mRenderer;
        LayerManager mLayerManager;

        RenderPass mDefaultRenderPass;

        std::unique_ptr<Gui> mGui;
        std::unique_ptr<Window> mWindow;
        std::deque<RenderPass*> mRenderPasses;
        
        Framegraph::Framegraph mFramegraph;
        
        Framegraph::TextureResource* mDepthTexture;
    };
}
