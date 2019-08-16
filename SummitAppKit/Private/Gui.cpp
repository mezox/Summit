#include <SummitAppKit/Gui.h>
#include <SummitAppKit/Window.h>
#include <Renderer/Renderer.h>
#include <Renderer/PointerEvent.h>
#include <Renderer/Effect.h>

#include "GuiCallback.h"

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

void ImGuiUpdateMonitors(bool& updateMonitors)
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    int monitors_count = 0;
    GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitors_count);
    platform_io.Monitors.resize(0);
    
    for (int n = 0; n < monitors_count; n++)
    {
        ImGuiPlatformMonitor monitor;
        int x, y;
        glfwGetMonitorPos(glfw_monitors[n], &x, &y);
        const GLFWvidmode* vid_mode = glfwGetVideoMode(glfw_monitors[n]);
#if GLFW_HAS_MONITOR_WORK_AREA
        monitor.MainPos = ImVec2((float)x, (float)y);
        monitor.MainSize = ImVec2((float)vid_mode->width, (float)vid_mode->height);
        int w, h;
        glfwGetMonitorWorkarea(glfw_monitors[n], &x, &y, &w, &h);
        monitor.WorkPos = ImVec2((float)x, (float)y);;
        monitor.WorkSize = ImVec2((float)w, (float)h);
#else
        monitor.MainPos = monitor.WorkPos = ImVec2((float)x, (float)y);
        monitor.MainSize = monitor.WorkSize = ImVec2((float)vid_mode->width, (float)vid_mode->height);
#endif
#if GLFW_HAS_PER_MONITOR_DPI
        // Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
        float x_scale, y_scale;
        glfwGetMonitorContentScale(glfw_monitors[n], &x_scale, &y_scale);
        monitor.DpiScale = x_scale;
#endif
        platform_io.Monitors.push_back(monitor);
    }
    
    updateMonitors = false;
}

namespace detail
{
    void UpdateViewport(const ImDrawData& imDrawData, ImGuiViewportData& data, Summit::CmdList& guiCmdList)
    {
        using namespace Summit;
        
        // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
        int32_t framebufferWidth = static_cast<int32_t>(imDrawData.DisplaySize.x * imDrawData.FramebufferScale.x);
        int32_t framebufferHeight = static_cast<int32_t>(imDrawData.DisplaySize.y * imDrawData.FramebufferScale.y);
        
        if (framebufferWidth <= 0 || framebufferHeight <= 0)
            return;
        
        if (imDrawData.TotalVtxCount == 0 || imDrawData.TotalIdxCount == 0)
            return;
        
        auto& vertexBuffer = data.vertexBuffer;
        auto& positionDataStream = vertexBuffer.GetDataStream<0>();
        auto& texCoordDataStream = vertexBuffer.GetDataStream<1>();
        auto& colorDataStream = vertexBuffer.GetDataStream<2>();
        auto& indexDataStream = vertexBuffer.GetIndexStream();
        
        positionDataStream.Discard(Renderer::DiscardMode::HostAndDevice);
        texCoordDataStream.Discard(Renderer::DiscardMode::HostAndDevice);
        colorDataStream.Discard(Renderer::DiscardMode::HostAndDevice);
        indexDataStream.Discard(Renderer::DiscardMode::HostAndDevice);
        
        auto& positionData = positionDataStream.GetData();
        auto& texCoordData = texCoordDataStream.GetData();
        auto& colorData = colorDataStream.GetData();
        auto& indexData = indexDataStream.GetData();
        
        positionData.reserve(imDrawData.TotalVtxCount);
        texCoordData.reserve(imDrawData.TotalVtxCount);
        colorData.reserve(imDrawData.TotalVtxCount);
        indexData.reserve(imDrawData.TotalIdxCount);
        
        ImVec2 clipOff = imDrawData.DisplayPos;         // (0,0) unless using multi-viewports
        ImVec2 clipScale = imDrawData.FramebufferScale; // (1,1) unless using retina display which are often (2,2)
        
        // Setup viewport, orthographic projection matrix
        // Our visible imgui space lies from draw_data->DisplayPos (top left) to
        // draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
        Summit::Viewport<double> viewport;
        viewport.rect.x = 0.0;
        viewport.rect.y = 0.0;
        viewport.rect.width = static_cast<double>(imDrawData.DisplaySize.x * imDrawData.FramebufferScale.x);
        viewport.rect.height = static_cast<double>(imDrawData.DisplaySize.y * imDrawData.FramebufferScale.y);
        viewport.near = 0.0;
        viewport.far = 1.0;
        
        guiCmdList.Record<SetViewportCommand>(std::move(viewport));
        
        float L = imDrawData.DisplayPos.x;
        float R = imDrawData.DisplayPos.x + imDrawData.DisplaySize.x;
        float T = imDrawData.DisplayPos.y;
        float B = imDrawData.DisplayPos.y + imDrawData.DisplaySize.y;
        float N = viewport.near;
        float F = viewport.far;

        float ortho_projection[4][4] =
        {
            { 2.0f/(R-L),   0.0f,           0.0f,   0.0f },
            { 0.0f,         2.0f/(T-B),     0.0f,   0.0f },
            { 0.0f,         0.0f,        1/(F-N),   0.0f },
            { (R+L)/(L-R),  (T+B)/(B-T), N/(F-N),   1.0f },
        };

        guiCmdList.Record<SetVertexBytesCommand>(&ortho_projection, 16 * sizeof(float), 3);
        
        size_t posBufferOffset{ 0 }, texCoordBufferOffset{ 0 }, colorBufferDataOffset{ 0 };
        size_t indexBufferOffset{ 0 };
        
        for (int32_t i{ 0 }; i < imDrawData.CmdListsCount; ++i)
        {
            const ImDrawList* cmd_list = imDrawData.CmdLists[i];
            const auto vertexCount = cmd_list->VtxBuffer.size();
            const auto indexCount = cmd_list->IdxBuffer.size();
            
            // Copy vertex data
            for (int32_t vertexId{ 0 }; vertexId < vertexCount; ++vertexId)
            {
                const auto& vertexData = cmd_list->VtxBuffer[vertexId];
                positionData.push_back({ vertexData.pos.x, vertexData.pos.y });
                texCoordData.push_back({ vertexData.uv.x, vertexData.uv.y });
                colorData.push_back(vertexData.col);
            }
            
            // Copy index data
            for (int32_t indexId{ 0 }; indexId < indexCount; ++indexId)
            {
                indexData.push_back(cmd_list->IdxBuffer[indexId]);
            }
            
            // Generate commands
            for (int cmd_i{ 0 }; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
                const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
                if (pcmd->UserCallback)
                {
                    if (pcmd->UserCallback == ImDrawCallback_ResetRenderState)
                    {
                        
                    }
                    else
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                }
                else
                {
                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec4 clipRect;
                    clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
                    clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
                    clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
                    clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;
                    
                    if (clipRect.x < framebufferWidth && clipRect.y < framebufferHeight && clipRect.z >= 0.0f && clipRect.w >= 0.0f)
                    {
                        Summit::Rect<uint32_t> scissorRect;
                        scissorRect.x = static_cast<uint32_t>(clipRect.x);
                        scissorRect.y = static_cast<uint32_t>(clipRect.y);
                        scissorRect.width = static_cast<uint32_t>(clipRect.z - clipRect.x);
                        scissorRect.height = static_cast<uint32_t>(clipRect.w - clipRect.y);
                        
                        guiCmdList.Record<SetScissorRectCommand>(std::move(scissorRect));
                        
                        // Set vertex buffer offset for each data stream
                        guiCmdList.Record<SetVertexBufferOffsetCommand>(posBufferOffset + pcmd->VtxOffset * sizeof(positionData[0]), 0);
                        guiCmdList.Record<SetVertexBufferOffsetCommand>(texCoordBufferOffset + pcmd->VtxOffset * sizeof(texCoordData[0]), 1);
                        guiCmdList.Record<SetVertexBufferOffsetCommand>(colorBufferDataOffset + pcmd->VtxOffset * sizeof(colorData[0]), 2);
                        
                        // TODO: This is just a hack, because at this point I do not have id of the buffer
                        guiCmdList.Record<DrawIndexedPrimitivesCommand>(BufferId{ 0 },
                                                                        sizeof(indexData[0]),
                                                                        pcmd->ElemCount,
                                                                        indexBufferOffset + pcmd->IdxOffset * sizeof(indexData[0]));
                    }
                }
            }
            
            posBufferOffset += cmd_list->VtxBuffer.Size * sizeof(positionData[0]);
            texCoordBufferOffset += cmd_list->VtxBuffer.Size * sizeof(texCoordData[0]);
            colorBufferDataOffset += cmd_list->VtxBuffer.Size * sizeof(colorData[0]);
            indexBufferOffset += cmd_list->IdxBuffer.Size * sizeof(indexData[0]);
        }
        
        // Map
        positionDataStream.Unlock();
        texCoordDataStream.Unlock();
        colorDataStream.Unlock();
        indexDataStream.Unlock();
    }
}

namespace Summit
{
    Gui::Gui(AppKit::Window* window, IRenderer* renderer)
        : mRenderer(*renderer)
    {
        mGuiContext = static_cast<void*>(ImGui::CreateContext());

        ImGuiIO& io = ImGui::GetIO();
        io.BackendRendererName = "Summit::MetalRenderer";

        // Enable features
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Define style
        auto& style = ImGui::GetStyle();
        style.FrameRounding = 0.f;
        style.ScrollbarRounding = 0.f;
        style.WindowRounding = 0.f;

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;

            io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
            io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;

            ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
            SummitImGui::SetViewportPlatformCallbacks(platform_io);
            SummitImGui::SetViewportRendererCallbacks(platform_io);

            // TODO: Get rid of GLFW, it's used only because of monitors
            if(!glfwInit())
                throw std::runtime_error("");

            bool update{ true };
            ImGuiUpdateMonitors(update);
        }

        // Register main window
        ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        ImGuiViewportData* data = IM_NEW(ImGuiViewportData)();
        data->window = window;
        data->isOwned = false;
        data->renderer = renderer;
        data->vertexBuffer.SetRenderer(renderer);
        data->vertexBuffer.SetMemoryType(MemoryType::HostVisible);

        main_viewport->PlatformUserData = data;
        main_viewport->PlatformHandle = (void*)window;
        main_viewport->PlatformHandleRaw = window->GetContentView()->GetNativeHandle();

        // Create font texture
        unsigned char* fontData{ nullptr };

        int texWidth{ 0 }, texHeight{ 0 };
        io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);

        mTexture = renderer->CreateTexture2D(texWidth, texHeight, (void*)fontData);

        // Create sampler
        mSampler = renderer->CreateSampler({});

        // Create Pipeline
        PipelineDescriptor guiEffect;
        guiEffect.AddModule(ModuleStage::Vertex, "vertex_main");
        guiEffect.AddModule(ModuleStage::Fragment, "fragment_main");
        guiEffect.AddAttribute(Format::Float2, 0);
        guiEffect.AddAttribute(Format::Float2, 1);
        guiEffect.AddAttribute(Format::UChar4, 2);
        guiEffect.AddResource(UniformType::Texture, ModuleStage::Fragment, 0, mTexture.id);
        guiEffect.AddResource(UniformType::Sampler, ModuleStage::Fragment, 0, mSampler.id);

        mPipelineId = renderer->CreatePipeline(guiEffect);

        data->window->DidMove.connect([window = data->window](const uint32_t x, const uint32_t y) {
            if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
                viewport->PlatformRequestMove = true;
        });

        data->window->MouseEvent.connect([](const PointerEvent& event) {
            ImGuiIO& io = ImGui::GetIO();

            switch (event.type)
            {
                case MouseEventType::LeftPress:
                    io.MouseDown[0] = true;
                    break;
                case MouseEventType::LeftRelease:
                    io.MouseDown[0] = false;
                    break;
                case MouseEventType::RightPress:
                    io.MouseDown[1] = true;
                    break;
                case MouseEventType::RightRelease:
                    io.MouseDown[1] = false;
                    break;

                default:
                    break;
            }
        });
    }

    Gui::~Gui()
    {
        ImGui::DestroyContext(static_cast<ImGuiContext*>(mGuiContext));

        mRenderer.ReleaseTexture(mTexture);
        mRenderer.ReleaseSampler(mSampler);
        mRenderer.ReleasePipeline(mPipelineId);
    }

    void Gui::BeginFrame(const FrameData& fd)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2{ (float)fd.width, (float)fd.height };

        if (fd.width > 0 && fd.height > 0)
        {
    //        const auto swapChainFramebufferWidth = swapChain.GetFramebufferWidth();
    //        const auto swapChainFramebufferHeight = swapChain.GetFramebufferHeight();
            //io.DisplayFramebufferScale = ImVec2{ (float)swapChainFramebufferWidth / swapChainImageWidth, (float)swapChainFramebufferHeight / swapChainImageHeight };
        }

        ImGui::NewFrame();

        // Update mouse position
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
        io.MouseHoveredViewport = 0;

        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        for (int n = 0; n < platform_io.Viewports.Size; n++)
        {
            ImGuiViewport* viewport = platform_io.Viewports[n];
            Summit::AppKit::Window* window = (Summit::AppKit::Window*)viewport->PlatformHandle;

            const bool focused = window->IsFocused();
            if (focused)
            {
                int32_t mouse_x, mouse_y;
                window->GetCursorPosition(mouse_x, mouse_y);

                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                {
                    // Multi-viewport mode: mouse position in OS absolute coordinates (io.MousePos is (0,0) when the mouse is on the upper-left of the primary monitor)
                    const auto windowPosition = window->GetPosition();
                    io.MousePos = ImVec2((float)mouse_x + windowPosition.x, (float)mouse_y + windowPosition.y);
                }
                else
                {
                    // Single viewport mode: mouse position in client window coordinates (io.MousePos is (0,0) when the mouse is on the upper-left corner of the app window)
                    io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);
                }
            }
        }

        for (int i = 1; i < platform_io.Viewports.Size; ++i)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)platform_io.Viewports[i]->PlatformUserData;
            data->window->GetContentView()->Acquire();
        }

        bool open = true;
        constexpr bool opt_fullscreen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        //
        //        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        //        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        //        // all active windows docked into it will lose their parent and become undocked.
        //        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        //        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Main View", &open, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();
    }

    void Gui::EndFrame()
    {
        ImGui::Render();

        const auto ProcessViewport = [this](const ImGuiViewport& viewport, auto&& presentFunc) {

            CmdList commandList;

            if(!viewport.PlatformUserData)
                return;

            auto* viewportData = static_cast<ImGuiViewportData*>(viewport.PlatformUserData);
            std::string passName = "UIPass_Viewport_" + viewportData->window->GetTitle();

            mRenderer.BeginRenderPass(*viewportData->window->GetContentView(), passName.c_str());
            detail::UpdateViewport(*viewport.DrawData, *viewportData, commandList);

            mRenderer.RenderObject3d(*viewportData, mPipelineId, &commandList);
            mRenderer.EndRenderPass();

            presentFunc(viewportData->window);
        };

        // Process main viewport
        ProcessViewport(*ImGui::GetMainViewport(), [](auto){});

        ImGuiIO& io = ImGui::GetIO();
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            for (int i = 1; i < platform_io.Viewports.Size; ++i)
            {
                ProcessViewport(*platform_io.Viewports[i], [](Summit::AppKit::Window* window){
                    window->GetContentView()->SwapBuffers();
                });
            }

            ImGui::UpdatePlatformWindows();
        }
    }
}
