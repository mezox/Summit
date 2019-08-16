#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"

namespace MTL
{
    /*!
     @brief The presented callback function protocol
     @disucssion Be careful when you use delta between this presentedTime and previous frame's presentedTime to animate next frame. If the frame was presented using presentAfterMinimumDuration or presentAtTime, the presentedTime might includes delays to meet your specified present time. If you want to measure how much frame you can achieve, use GPUStartTime in the first command buffer of your frame rendering and GPUEndTime of your last frame rendering to calculate the frame interval.
     */
    using DrawablePresentedHandler = std::function<void(class Drawable&)>;
    
    /*!
     @brief All "drawable" objects (such as those coming from CAMetalLayer) are expected to conform to this protocol
     */
    class METAL_CPP_API Drawable final : public NSHandleWrapper
    {
        friend class CommandBuffer;
        
    public:
        Drawable() = default;
        
        explicit Drawable(void* handle) noexcept;
        
        /*!
         @abstract Present this drawable as soon as possible
         */
        void Present() const;
        
        /*!
         @abstract Present this drawable at the given host time
         */
        void PresentAtTime(double presentationTime) const;
    };
}
