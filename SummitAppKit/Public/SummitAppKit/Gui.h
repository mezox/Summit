#pragma once

#include <Renderer/RendererTypes.h>
#include <cstdint>

struct FrameData
{
    uint32_t width{ 0 };
    uint32_t height{ 0 };
};

namespace Summit
{
    class IRenderer;

    namespace AppKit
    {
        class Window;
    }

    class Gui
    {
    public:
        Gui(AppKit::Window* window, IRenderer* renderer);
        ~Gui();

        void BeginFrame(const FrameData& fd);
        void EndFrame();

    private:
        void* mGuiContext{ nullptr };
        IRenderer& mRenderer;

        TextureId mTexture;
        PipelineId mPipelineId;
        SamplerId mSampler;
    };
}
