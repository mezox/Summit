#pragma once

#include <type_traits>

namespace Summit
{
    class BeginCommand;
    class SetViewportCommand;
    class SetScissorRectCommand;
    class SetVertexBufferOffsetCommand;
    class DrawIndexedPrimitivesCommand;
    class SetVertexBytesCommand;
    
    class ICommandVisitor
    {
    public:
        virtual ~ICommandVisitor() = default;
        
        virtual void Visit(const BeginCommand& cmd) = 0;
        virtual void Visit(const SetViewportCommand& cmd) = 0;
        virtual void Visit(const SetScissorRectCommand& cmd) = 0;
        virtual void Visit(const SetVertexBufferOffsetCommand& cmd) = 0;
        virtual void Visit(const DrawIndexedPrimitivesCommand& cmd) = 0;
        virtual void Visit(const SetVertexBytesCommand& cmd) = 0;
    };
    
    class ICommandImpl
    {
    public:
        virtual ~ICommandImpl() = default;
        
        virtual void Execute(ICommandVisitor& visitor) const = 0;
        virtual ICommandImpl* Move(void* address) = 0;
        virtual const char* GetDescription() const = 0;
    };
    
    template<typename T>
    class CommandImpl final : public ICommandImpl
    {
    public:
        CommandImpl(T v) noexcept
        : data(std::move(v))
        {}
        
        ICommandImpl* Move(void* addr) override
        {
            return new (addr) CommandImpl(std::move(*this));
        }
        
        void Execute(ICommandVisitor& visitor) const override
        {
            visitor.Visit(data);
        }
        
        const char* GetDescription() const override
        {
            return data.GetDescription();
        }
        
    public:
        T data;
    };
    
    class Command
    {
    public:
        Command() = default;
        
        Command(const Command& other) = delete;
        Command(Command&& other) noexcept;
        
        Command& operator=(const Command& other) = delete;
        Command& operator=(Command&& other) noexcept;
        
        template<typename T>
        static Command Create(T&& command)
        {
            Command instance;
            CommandImpl<T>* impl = new (&instance.mStorage) CommandImpl<T>(std::forward<T>(command));
            instance.mImpl = (ICommandImpl*)((char*)&instance.mStorage);
            return instance;
        }
        
        void Execute(ICommandVisitor& visitor) const;
        const char* GetDescription() const noexcept;
        
    private:
        std::aligned_storage<64, 16>::type mStorage;
        ICommandImpl* mImpl{ nullptr };
    };
}
