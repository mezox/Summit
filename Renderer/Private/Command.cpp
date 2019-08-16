#include <Renderer/Command.h>

namespace Summit
{
    Command::Command(Command&& other) noexcept
        : mImpl(other.mImpl->Move(&mStorage))
    {}
    
    Command& Command::operator=(Command&& other) noexcept
    {
        mImpl->~ICommandImpl();
        mImpl = other.mImpl->Move(&mStorage);
        other.mImpl->~ICommandImpl();
        return *this;
    }
    
    void Command::Execute(ICommandVisitor& visitor) const
    {
        mImpl->Execute(visitor);
    }
    
    const char* Command::GetDescription() const noexcept
    {
        return mImpl->GetDescription();
    }
}
