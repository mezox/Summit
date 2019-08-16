#pragma once

#include <cstdint>
#include "Renderer.h"

#define FORCE_INLINE inline __attribute__ ((always_inline))

namespace Summit::Renderer
{
    class IVisitor
    {
    public:
        virtual ~IVisitor() = default;
    };

    class IRendererResource
    {
    public:
        virtual ~IRendererResource() = default;

        virtual void InitializeDeviceResource(IRenderer& renderer) = 0;
        virtual void ReleaseDeviceResource(IRenderer& renderer) = 0;
    };
}
