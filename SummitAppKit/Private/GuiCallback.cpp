#include "GuiCallback.h"

#include <SummitAppKit/Window.h>

#include <imgui/imgui.h>

namespace SummitImGui
{
    void SetViewportPlatformCallbacks(ImGuiPlatformIO& platform_io)
    {
        //    platform_io.Platform_SetWindowFocus = ImGui_ImplGlfw_SetWindowFocus;
        //    platform_io.Platform_GetWindowFocus = ImGui_ImplGlfw_GetWindowFocus;
        //    platform_io.Platform_GetWindowMinimized = ImGui_ImplGlfw_GetWindowMinimized;
        
        platform_io.Platform_CreateWindow = [](ImGuiViewport* viewport)
        {
            //        ASSERT(data != nullptr);
            //        ASSERT(data->window == nullptr);
            //        ASSERT(data->renderer != nullptr);
            
            const auto* mainViewport = ImGui::GetMainViewport();
            const auto* mainViewportData = static_cast<ImGuiViewportData*>(mainViewport->PlatformUserData);
            
            auto* data = IM_NEW(ImGuiViewportData)();
            
            data->window = new Summit::AppKit::Window("SummitAppKit window", viewport->Size.x, viewport->Size.y, mainViewportData->renderer);
            data->isOwned = true;
            
            viewport->PlatformHandle = (void*)data->window;
            viewport->PlatformHandleRaw = data->window->GetContentView()->GetNativeHandle();
            
            // Setup window
            data->window->SetPosition(Point2i(viewport->Pos.x, viewport->Pos.y));
            
            data->window->Resize.connect([window = data->window](const uint32_t x, const uint32_t y) {
                if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(window))
                    viewport->PlatformRequestResize = true;
            });
            
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
            
            // Setup vertex buffer
            data->vertexBuffer.SetRenderer(mainViewportData->renderer);
            data->vertexBuffer.SetMemoryType(Summit::MemoryType::HostVisible);
            
            viewport->PlatformUserData = data;
        };
        
        platform_io.Platform_DestroyWindow = [](ImGuiViewport* viewport)
        {
            if (ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData)
            {
                if(data->isOwned)
                {
                    delete data->window;
                }
                
                IM_DELETE(data);
            }
            
            viewport->PlatformUserData = viewport->PlatformHandle = nullptr;
        };
        
        platform_io.Platform_ShowWindow = [](ImGuiViewport* viewport)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            data->window->Show();
        };
        
        platform_io.Platform_SetWindowPos = [](ImGuiViewport* viewport, ImVec2 pos)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            data->window->SetPosition(Point2i(pos.x, pos.y));
        };
        
        platform_io.Platform_GetWindowPos = [](ImGuiViewport* viewport) {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            
            Point2i position = data->window->GetPosition();
            return ImVec2((float)position.x, (float)position.y);
        };
        
        platform_io.Platform_SetWindowSize = [](ImGuiViewport* viewport, ImVec2 size)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            data->window->SetSize((uint32_t)size.x, (uint32_t)size.y);
        };
        
        platform_io.Platform_GetWindowSize = [](ImGuiViewport* viewport)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            
            uint32_t width{ 0 }, height{ 0 };
            
            data->window->GetSize(width, height);
            return ImVec2((float)width, (float)height);
        };
        
        platform_io.Platform_SetWindowTitle = [](ImGuiViewport* viewport, const char* title)
        {
            ImGuiViewportData* data = (ImGuiViewportData*)viewport->PlatformUserData;
            data->window->SetTitle(title);
        };
    }
    
    void SetViewportRendererCallbacks(ImGuiPlatformIO& platform_io)
    {
        //        [](ImGuiViewport* viewport, void* data)
        //        {
        //            if (!(viewport->Flags & ImGuiViewportFlags_NoRendererClear))
        //            {
        //                ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        //                //            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        //                //            glClear(GL_COLOR_BUFFER_BIT);
        //            }
        //
        //            //ImGui_ImplOpenGL3_RenderDrawData(viewport->DrawData);
        //        }
        
        //    platform_io.Renderer_RenderWindow = InternalImGui::Callbacks::ImGuiRenderWindow;
        //    platform_io.Platform_SwapBuffers = ImGui_ImplGlfw_SwapBuffers;
        //    platform_io.Platform_SetWindowAlpha = ImGui_ImplGlfw_SetWindowAlpha;
    }
}
