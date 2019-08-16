#pragma once

#include "MetalCppModule.h"
#include "MetalWrapperCommon.h"
#include "MetalDeviceResource.h"

#include <cstdint>

namespace MTL
{
    class RenderPipelineState;
    class Buffer;
    class Texture;
    class Buffer;
    class SamplerState;
    class DepthStencilState;
    class Heap;
    
    enum class PrimitiveType
    {
        Point,
        Line,
        LineStrip,
        Triangle,
        TriangleStrip,
    };
    
    enum class IndexType
    {
        UInt16,
        UInt32
    };
    
    enum class CullMode
    {
        None,
        Front,
        Back
    };
    
    enum class Winding
    {
        Clockwise,
        CounterClockwise
    };
    
    /*!
     * @brief Describes how a resource will be used by a shader through an argument buffer
     */
    enum class ResourceUsage
    {
        Read   = 1 << 0,
        Write  = 1 << 1,
        Sample = 1 << 2
    };
    
    struct ScissorRect
    {
        uint32_t x{ 0 };
        uint32_t y{ 0 };
        uint32_t width{ 0 };
        uint32_t height{ 0 };
    };
    
    struct Viewport
    {
        double originX{ 0.0 };
        double originY{ 0.0 };
        double width{ 0.0 };
        double height{ 0.0 };
        double znear{ 0.0 };
        double zfar{ 0.0 };
    };
    
    class METAL_CPP_API CommandEncoder : public NSHandleWrapper, public DeviceResource<CommandEncoder>
    {
        DECLARE_METAL_DEVICE_RESOURCE_INTERFACE
        
    public:
        CommandEncoder() = default;
        explicit CommandEncoder(void* handle);
        virtual ~CommandEncoder() = default;
        
        CommandEncoder& operator=(CommandEncoder&& other) = default;
        
        /*!
         @abstract Declare that all command generation from this encoder is complete, and detach from the MTLCommandBuffer.
         */
        void EndEncoding() const noexcept;
        
        /* Debug Support */
        
        /*!
         @abstract Inserts a debug string into the command buffer.  This does not change any API behavior, but can be useful when debugging.
         */
        void InsertDebugSignpost(const char* string) const noexcept;
        
        /*!
         @abstract Push a new named string onto a stack of string labels.
         */
        void PushDebugGroup(const char* string) const noexcept;
        
        /*!
         @abstract Pop the latest named string off of the stack.
         */
        void PopDebugGroup() const noexcept;
    };
    
    class METAL_CPP_API RenderCommandEncoder final : public CommandEncoder
    {
    public:
        RenderCommandEncoder() = default;
        explicit RenderCommandEncoder(void* handle);
        
        void SetRenderPipelineState(const RenderPipelineState& state) const;
        
        /*!
         @brief Set the viewport, which is used to transform vertexes from normalized device coordinates to window coordinates.  Fragments that lie outside of the viewport are clipped, and optionally clamped for fragments outside of znear/zfar.
         */
        void SetViewport(const Viewport& viewport) const noexcept;
        
        /*!
         @brief Specifies a rectangle for a fragment scissor test.  All fragments outside of this rectangle are discarded.
         */
        void SetScissorRect(const ScissorRect& rect) const noexcept;
        
        /*!
         @brief Set the offset within the current global buffer for all vertex shaders at the given bind point index.
         */
        void SetVertexBufferOffset(uint32_t offset, uint32_t index) const noexcept;
        
        /*!
         @brief Set the DepthStencil state object.
         */
        void SetDepthStencilState(const DepthStencilState& depthStencilState) const noexcept;
        
        void SetVertexBuffer(const Buffer& buffer, const uint32_t offset, const uint32_t index) const;
        
        /*!
         @brief Set the data (by copy) for a given vertex buffer binding point.  This will remove any existing MTLBuffer from the binding point.
         */
        void SetVertexBytes(const void* bytes, const uint32_t length, const uint32_t index) const noexcept;
        
        
        void DrawPrimitives(const PrimitiveType type, const uint32_t vertexStart, const uint32_t vertexCount) const;
        
        /*!
         @brief Draw primitives with an index list.
         @param primitiveType The type of primitives that elements are assembled into.
         @param indexCount The number of indexes to read from the index buffer for each instance.
         @param indexType The type if indexes, either 16 bit integer or 32 bit integer.
         @param indexBuffer A buffer object that the device will read indexes from.
         @param indexBufferOffset Byte offset within @a indexBuffer to start reading indexes from.  @a indexBufferOffset must be a multiple of the index size.
         */
        void DrawIndexedPrimitives(const PrimitiveType type, const uint32_t indexCount, IndexType indexType, const Buffer& indexBuffer, const uint32_t indexBufferOffset) const noexcept;
        
        /*!
         @brief Draw primitives with an index list.
         @param primitiveType The type of primitives that elements are assembled into.
         @param indexCount The number of indexes to read from the index buffer for each instance.
         @param indexType The type if indexes, either 16 bit integer or 32 bit integer.
         @param indexBuffer A buffer object that the device will read indexes from.
         @param indexBufferOffset Byte offset within @a indexBuffer to start reading indexes from.  @a indexBufferOffset must be a multiple of the index size.
         @param instanceCount The number of instances drawn.
         */
        void DrawIndexedPrimitives(PrimitiveType type, uint32_t indexCount, IndexType indexType, const Buffer& indexBuffer, uint32_t indexBufferOffset, uint32_t instanceCount) const noexcept;
        
        /*!
         @brief Set a global buffer for all fragment shaders at the given bind point index.
         */
        void SetFragmentBuffer(const Buffer& buffer, const uint32_t offset, const uint32_t index) const noexcept;
        
        /*!
         @brief Set a global texture for all fragment shaders at the given bind point index.
         */
        void SetFragmentTexture(const Texture& texture, const uint32_t index) const noexcept;
        
        /*!
         @brief Set a global sampler for all fragment shaders at the given bind point index.
         */
        void SetFragmentSamplerState(const SamplerState& samplerState, const uint32_t index) const noexcept;
        
        /*!
         @brief Set the winding order of front-facing primitives.
         */
        void SetFrontFacingWinding(Winding frontFacingWinding) const noexcept;
        
        /*!
         @brief Controls if primitives are culled when front facing, back facing, or not culled at all.
         */
        void SetCullMode(CullMode cullMode) const noexcept;
        
        /*!
         * @brief Declare that a resource may be accessed by the render pass through an argument buffer
         * @discussion This method does not protect against data hazards; these hazards must be addressed using an MTLFence. This method must be called before encoding any draw commands which may access the resource through an argument buffer. However, this method may cause color attachments to become decompressed. Therefore, this method should be called until as late as possible within a render command encoder. Declaring a minimal usage (i.e. read-only) may prevent color attachments from becoming decompressed on some devices.
         
         Note that calling useResource does not retain the resource. It is the responsiblity of the user to retain the resource until
         the command buffer has been executed.
         */
        void UseResource(const void* resource, ResourceUsage usage) const noexcept;
        
        /*!
         * @brief Declare that the resources allocated from a heap may be accessed by the render pass through an argument buffer
         * @discussion This method does not protect against data hazards; these hazards must be addressed using an MTLFence. This method must be called before encoding any draw commands which may access the resources allocated from the heap through an argument buffer. This method may cause all of the color attachments allocated from the heap to become decompressed. Therefore, it is recommended that the useResource:usage: or useResources:count:usage: methods be used for color attachments instead, with a minimal (i.e. read-only) usage.
         */
        void UseHeap(const Heap& heap) const noexcept;
    };
    
    
    class METAL_CPP_API BlitEncoder final : public CommandEncoder
    {
    public:
        BlitEncoder() = delete;
        
        explicit BlitEncoder(void* handle);
        
        /*!
         @abstract Basic memory copy between buffers.
         */
        void CopyFromBuffer(const Buffer& sourceBuffer, uint32_t sourceOffset, const Buffer& destinationBuffer, uint32_t destinationOffset, uint32_t size) const noexcept;
        
        /*!
         @brief Generate mipmaps for a texture from the base level up to the max level.
         */
        void GenerateMipmaps(const Texture& texture) const noexcept;
    };
}
