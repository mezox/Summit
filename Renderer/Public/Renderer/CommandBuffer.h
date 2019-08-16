#pragma once

#include <vector>
#include "Command.h"
#include "RendererTypes.h"

#include <iostream>

namespace Summit
{
    template<typename Derived>
    struct RenderCommand
    {
        void Execute() const
        {
            auto& derived = static_cast<const Derived&>(*this);
            derived.OnExecute();
        }
    };
    
    class BeginCommand final : public RenderCommand<BeginCommand>
    {
    public:
        BeginCommand()
        {}
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "BeginCommandBuffer";
        }
        
        void OnExecute() const
        {
        }
    };
    
    class SetViewportCommand final : public RenderCommand<SetViewportCommand>
    {
    public:
        SetViewportCommand(double x, double y, double width, double height, double near, double far)
        {
            viewport.rect.x = x;
            viewport.rect.y = y;
            viewport.rect.width = width;
            viewport.rect.height = height;
            viewport.near = near;
            viewport.far = far;
        }
        
        explicit SetViewportCommand(Viewport<double>&& vp)
            : viewport(std::move(vp))
        {}
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "SetViewportCommand";
        }
        
        void OnExecute() const
        {
        }
        
    public:
        Viewport<double> viewport;
    };
    
    class SetScissorRectCommand final : public RenderCommand<SetScissorRectCommand>
    {
    public:
        SetScissorRectCommand(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            rect.x = x;
            rect.y = y;
            rect.width = width;
            rect.height = height;
        }
        
        explicit SetScissorRectCommand(Rect<uint32_t>&& sr)
            : rect(std::move(sr))
        {}
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "SetViewportCommand";
        }
        
        void OnExecute(ICommandVisitor& visitor) const
        {
            
        }
        
    public:
        Rect<uint32_t> rect;
    };
    
    class SetVertexBytesCommand final : public RenderCommand<SetVertexBytesCommand>
    {
    public:
        SetVertexBytesCommand(void* d, const uint32_t dataSize, const uint32_t bufferIndex)
            : bufIdx(bufferIndex)
        {
            data.resize(dataSize);
            memcpy(data.data(), d, dataSize);
        }
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "SetVertexBytesCommand";
        }
        
        void OnExecute(ICommandVisitor& visitor) const
        {
            
        }
        
    public:
        std::vector<uint8_t> data;
        const uint32_t bufIdx;
    };
    
    class SetVertexBufferOffsetCommand final : public RenderCommand<SetVertexBufferOffsetCommand>
    {
    public:
        SetVertexBufferOffsetCommand(uint32_t offset, uint32_t bufferIndex)
            : offset(offset), index(bufferIndex)
        {}
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "SetVertexBufferOffsetCommand";
        }
        
        void OnExecute() const
        {
        }
        
    public:
        const uint32_t offset{ 0 };
        const uint32_t index{ 0 };
    };
    
    class DrawIndexedPrimitivesCommand final : public RenderCommand<DrawIndexedPrimitivesCommand>
    {
    public:
        DrawIndexedPrimitivesCommand(const BufferId indexBuffer, const uint8_t indexStride, const uint32_t indexCount, const uint32_t indexOffset)
            : buffer(indexBuffer)
            , stride(indexStride)
            , count(indexCount)
            , offset(indexOffset)
        {}
        
        [[nodiscard]] const char* GetDescription() const noexcept
        {
            return "DrawIndexedPrimitivesCommand";
        }
        
        void OnExecute() const
        {
        }
        
    public:
        const BufferId buffer;
        const uint8_t stride{ 4 };
        const uint32_t count{ 0 };
        const uint32_t offset{ 0 };
    };
    
    class CmdList final
    {
    public:
        CmdList() = default;
        ~CmdList() = default;
        
        template<typename Cmd, typename... Args>
        void Record(Args&&... args)
        {
            mCommands.push_back(Command::Create(Cmd(std::forward<Args>(args)...)));
        }
        
        template<typename Cmd>
        void Record(Cmd&& cmd)
        {
            mCommands.push_back(Command::Create(std::move(cmd)));
        }
        
        void Reset()
        {
            mCommands.clear();
        }
        
    public:
        std::vector<Command> mCommands;
    };
}
