#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"

namespace MTL
{
    class Texture;
    
    struct ClearColor
    {
        ClearColor(float r, float g, float b, float a)
            : red(r), green(g), blue(b), alpha(a)
        {}
        
        float red;
        float green;
        float blue;
        float alpha;
    };
    
    enum class LoadAction
    {
        DontCare,
        Load,
        Clear
    };
    
    enum class StoreAction
    {
        DontCare,
        Store,
        MultisampleResolve,
        StoreAndMultisampleResolve,
        Unknown,
        CustomSampleDepthStore
    };
    
    class METAL_CPP_API RenderPassAttachmentDescriptor : protected NSHandleWrapper
    {
    public:
        RenderPassAttachmentDescriptor();
        explicit RenderPassAttachmentDescriptor(void* handle);
        
        RenderPassAttachmentDescriptor& operator=(const RenderPassAttachmentDescriptor& other) = delete;
        
        /*!
         @property texture
         @abstract The MTLTexture object for this attachment.
         */
        void SetTexture(const Texture& texture);
        
        /*!
         @abstract The action to be performed with this attachment at the beginning of a render pass.  Default is
         MTLLoadActionDontCare unless specified by a creation or init method.
         */
        void SetLoadAction(LoadAction action);
        
        /*!
         @abstract The action to be performed with this attachment at the end of a render pass.  Default is
         MTLStoreActionDontCare unless specified by a creation or init method.
         */
       void SetStoreAction(StoreAction action);
    };
    
    class METAL_CPP_API RenderPassColorAttachmentDescriptor final : public RenderPassAttachmentDescriptor
    {
    public:
        explicit RenderPassColorAttachmentDescriptor(void* handle);
        
        /*!
         @abstract The clear color to be used if the loadAction property is MTLLoadActionClear
         */
        void SetClearColor(const ClearColor& color);
    };
    
    class METAL_CPP_API RenderPassDepthAttachmentDescriptor final : public RenderPassAttachmentDescriptor
    {
    public:
        explicit RenderPassDepthAttachmentDescriptor(void* handle);
        
        /*!
         @abstract The clear depth value to be used if the loadAction property is MTLLoadActionClear
         */
        void SetClearDepth(double value);
    };
    
    /*!
     @abstract MetalRenderPassDescriptor represents a collection of attachments to be used to create a concrete render command encoder
     */
    class METAL_CPP_API RenderPassDescriptor final : public NSHandleWrapper
    {
        friend class MetalCommandBuffer;
        
    public:
        RenderPassDescriptor();
        RenderPassDescriptor(void* handle);
        
        MTLArrayWrapper<RenderPassColorAttachmentDescriptor> GetColorAttachments() const noexcept;
        RenderPassDepthAttachmentDescriptor GetDepthAttachment() const noexcept;
    };
}

