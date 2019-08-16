#pragma once

#include <Renderer/Object3d.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/Math.h>
#include <Renderer/Math/Math.h>


namespace Summit
{
    class IRenderer;
    
    namespace AppKit
    {
        class Window;
    }
}

class ImGuiViewportData : public Summit::Renderer::Object3d
{
public:
    const Summit::Renderer::VertexBufferBase* GetVertexBufferPointer() const override
    {
        return &vertexBuffer;
    }
    
public:
    Summit::AppKit::Window* window{ nullptr };
    Summit::IRenderer* renderer{ nullptr };
    bool isOwned{ false };
    Summit::Renderer::VertexBuffer<Math::Float2, Math::Float2, uint32_t> vertexBuffer;
};

struct ImGuiPlatformIO;

namespace SummitImGui
{
    void SetViewportPlatformCallbacks(ImGuiPlatformIO& platform_io);
    void SetViewportRendererCallbacks(ImGuiPlatformIO& platform_io);
}
