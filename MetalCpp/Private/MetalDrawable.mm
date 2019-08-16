#include <pch.h>

#include <MetalCpp/MetalDrawable.h>

namespace MTL
{
    Drawable::Drawable(void* handle) noexcept
        : NSHandleWrapper(handle)
    {}

    void Drawable::Present() const
    {
        [(__bridge id<MTLDrawable>)mNativeHandle present];
    }

    void Drawable::PresentAtTime(double presentationTime) const
    {
        [(__bridge id<MTLDrawable>)mNativeHandle presentAtTime:static_cast<CFTimeInterval>(presentationTime)];
    }
}
