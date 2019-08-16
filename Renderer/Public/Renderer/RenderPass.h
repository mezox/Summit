#pragma once

#include "Signal.h"

namespace Summit
{
    /*!
     @brief A render pass represents a collection of attachments, subpasses
            and dependencies between the subpasses and describes how the attachments
            are used over the course of subpasses.
     */
    class RenderPass
    {
    public:
        RenderPass() = default;
        virtual ~RenderPass() = default;

        RenderPass(const RenderPass& other) = delete;
        RenderPass(RenderPass&& other) = delete;
        RenderPass& operator=(const RenderPass& other) = delete;
        RenderPass& operator=(RenderPass&& other) = delete;

    public:
        sigslot::signal<> Update;
    };
}
