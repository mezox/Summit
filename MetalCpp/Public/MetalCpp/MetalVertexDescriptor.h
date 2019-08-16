#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"

namespace MTL
{
    /*!
     @brief specifies how the vertex attribute data is laid out in memory.
     */
    enum class VertexFormat
    {
        Invalid,
        UChar2,
        UChar3,
        UChar4,
        Float,
        Float2,
        Float3,
        Float4,
        
        // TODO::Fill all in
    };
    
    enum class VertexStepFunction
    {
        Constant,
        PerVertex,
        PerInstance,
        PerPatch,               // API_AVAILABLE(macos(10.12), ios(10.0))
        PerPatchControlPoint    // API_AVAILABLE(macos(10.12), ios(10.0))
    };
    
    class METAL_CPP_API VertexAttributeDescriptor final : private NSHandleWrapper
    {
    public:
        VertexAttributeDescriptor();
        explicit VertexAttributeDescriptor(void* handle);
        VertexAttributeDescriptor(const VertexFormat format, const uint32_t bufferIndex, const uint32_t offset);
        
        VertexAttributeDescriptor& operator=(const VertexAttributeDescriptor& other) = delete;
        VertexAttributeDescriptor& operator=(VertexAttributeDescriptor&& other);

    public:
        void SetVertexFormat(VertexFormat format) noexcept;
        void SetOffset(uint32_t offset) noexcept;
        void SetBufferIntex(uint32_t bufferIndex) noexcept;
        
        [[nodiscard]] VertexFormat GetVertexFormat() const noexcept;
        [[nodiscard]] uint32_t GetOffset() const noexcept;
        [[nodiscard]] uint32_t GetBufferIntex() const noexcept;
    };
    
    class METAL_CPP_API VertexBufferLayoutDescriptor final : private NSHandleWrapper
    {
    public:
        VertexBufferLayoutDescriptor();
        explicit VertexBufferLayoutDescriptor(void *handle);
        VertexBufferLayoutDescriptor(const uint32_t stride, const VertexStepFunction stepFunc = VertexStepFunction::PerVertex, const uint32_t stepRate = 1);
        
        VertexBufferLayoutDescriptor& operator=(const VertexBufferLayoutDescriptor& other) = delete;
        VertexBufferLayoutDescriptor& operator=(VertexBufferLayoutDescriptor&& other);
        
        void SetStride(const uint32_t stride) noexcept;
        void SetVertexStepFunction(const VertexStepFunction stepFunc = VertexStepFunction::PerVertex) noexcept;
        void SetStepRate(const uint32_t stepRate = 1) noexcept;
        
        [[nodiscard]] uint32_t GetStride() const noexcept;
        [[nodiscard]] VertexStepFunction GetStepFunction() const noexcept;
        [[nodiscard]] uint32_t GetStepRate() const noexcept;
    };
    
    class METAL_CPP_API VertexDescriptor final : private NSHandleWrapper
    {
        friend class RenderPipelineDescriptor;
        
    public:
        VertexDescriptor();
        explicit VertexDescriptor(void* handle);
    
        MTLArrayWrapper<VertexAttributeDescriptor> GetAttributes() const noexcept;
        MTLArrayWrapper<VertexBufferLayoutDescriptor> GetLayouts() const noexcept;
        void Reset() noexcept;
    };
}
