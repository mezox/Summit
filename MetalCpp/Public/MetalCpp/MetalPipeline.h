#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalDeviceResource.h"

#include <string>
#include <vector>

namespace MTL
{
    class MetalDevice;
    class VertexDescriptor;
    class Buffer;
    class Texture;
    class SamplerState;
    
    enum class PixelFormat;
    
    /*!
     @brief An identifier for a top-level Metal function.
     @discussion Each location in the API where a program is used requires a function written for that specific usage.
     */
    enum class FunctionType
    {
        /*!
         A vertex shader, usable for a MTLRenderPipelineState.
         */
        Vertex,
         
        /*!
         A fragment shader, usable for a MTLRenderPipelineState.
         */
        Fragment,
         
        /*!
         A compute kernel, usable to create a MTLComputePipelineState.
         */
        Kernel
    };
    
    class METAL_CPP_API ArgumentEncoder final : public NSHandleWrapper
    {
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        
    public:
        explicit ArgumentEncoder(void* handle) noexcept;
        
        /*!
         * @brief The number of bytes required to store the encoded resource bindings.
         */
        uint32_t GetEncodedLength() const noexcept;
        
        /*!
         * @brief The alignment in bytes required to store the encoded resource bindings.
         */
        uint32_t GetAlignment() const noexcept;
        
        /*!
         * @brief Sets the destination buffer and offset at which the arguments will be encoded.
         */
       void SetArgumentBuffer(const Buffer& argumentBuffer, uint32_t offset) const noexcept;
        
//        /*!
//         * @brief Set a buffer at the given bind point index.
//         */
//        - (void)setBuffer:(nullable id <MTLBuffer>)buffer offset:(NSUInteger)offset atIndex:(NSUInteger)index;
//
        /*!
         * @brief Set a texture at the given bind point index.
         */
        void SetTexture(const Texture& texture, uint32_t index) const noexcept;

        /*!
         * @brief Set a sampler at the given bind point index.
         */
        void SetSamplerState(const SamplerState& state, uint32_t index) const noexcept;

        /*!
         * @brief Returns a pointer to the constant data at the given bind point index.
         */
        void* GetConstantData(uint32_t index) const noexcept;
    };
    
    class METAL_CPP_API Function final : public NSHandleWrapper
    {
    public:
        explicit Function(void* handle) noexcept;
        
        /*!
         @brief The overall kind of entry point: compute, vertex, or fragment.
         */
        FunctionType GetFunctionType() const noexcept;
        
        /*!
         * @brief Creates an argument encoder which will encode arguments matching the layout of the argument buffer at the given bind point index.
         */
        ArgumentEncoder NewArgumentEncoder(uint32_t bufferIndex) const noexcept;
    };
    
    class METAL_CPP_API Library final : public DeviceResource<Library>
    {
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        
    public:
        Library(void* handle);
        ~Library();
        
        /*!
         @abstract Returns a pointer to a function object, or nothing if the function is not found in the library.
         */
        Function CreateNewFunctionWithName(const std::string& functionName) const;
        
        /*!
         @abstract The array contains std::String objects, with the name of each function in library.
         */
        std::vector<std::string> GetFunctionNames() const noexcept;
        
    private:
        void* mNativeHandle{ nullptr };
    };
    
    enum class BlendFactor
    {
        Zero,
        One,
        SourceColor,
        OneMinusSourceColor,
        SourceAlpha,
        OneMinusSourceAlpha,
        DestinationColor,
        OneMinusDestinationColor,
        DestinationAlpha,
        OneMinusDestinationAlpha,
        SourceAlphaSaturated,
        BlendColor,
        OneMinusBlendColor,
        BlendAlpha,
        OneMinusBlendAlpha,
        Source1Color,                   //API_AVAILABLE(macos(10.12), ios(10.11)) = 15,
        OneMinusSource1Color,           //API_AVAILABLE(macos(10.12), ios(10.11)) = 16,
        Source1Alpha,                   //API_AVAILABLE(macos(10.12), ios(10.11)) = 17,
        OneMinusSource1Alpha            //API_AVAILABLE(macos(10.12), ios(10.11)) = 18,
    };
    
    enum class BlendOperation
    {
        Add,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };
    
    class METAL_CPP_API RenderPipelineColorAttachmentDescriptor final
    {
    public:
        RenderPipelineColorAttachmentDescriptor();
        explicit RenderPipelineColorAttachmentDescriptor(void* handle);
        ~RenderPipelineColorAttachmentDescriptor();
        
        void EnableBlending(bool toggle) noexcept;
        void SetSourceRGBBlendFactor(BlendFactor factor) noexcept;
        void SetDestinationRGBBlendFactor(BlendFactor factor) noexcept;
        void SetRGBBlendOperation(BlendOperation op) noexcept;
        void SetSourceAlphaBlendFactor(BlendFactor factor) noexcept;
        void SetDestinationAlphaBlendFactor(BlendFactor factor) noexcept;
        void SetAlphaBlendOperation(BlendOperation op) noexcept;
        
    private:
        void* mNativeHandle{ nullptr };
    };
    
    class METAL_CPP_API RenderPipelineDescriptor final : private NSHandleWrapper
    {
        friend class MetalDevice;
        
    public:
        RenderPipelineDescriptor();
        explicit RenderPipelineDescriptor(void* handle);
        
        void SetLabel(const std::string& label) const;
        void SetVertexFunction(const Function& f) const;
        void SetFragmentFunction(const Function& f) const;
        void SetVertexDescriptor(const VertexDescriptor& desc) const noexcept;
        void SetDepthAttachmentPixelFormat(const PixelFormat format) const noexcept;
        void SetStencilAttachmentPixelFormat(const PixelFormat format) const noexcept;
        
        MTLArrayWrapper<RenderPipelineColorAttachmentDescriptor> GetAttachments() const noexcept;
    };
    
    class METAL_CPP_API RenderPipelineState final : public NSHandleWrapper
    {
    public:
        RenderPipelineState() = delete;
        explicit RenderPipelineState(void* handle);
    };
}
