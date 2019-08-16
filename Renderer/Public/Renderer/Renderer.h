#pragma once

#include <memory>
#include <string>
#include <vector>

#include "SwapChain.h"
#include "DeviceResourceId.h"
#include "CommandBuffer.h"
#include "RendererTypes.h"

#include "Resources/Texture.hpp"

namespace Summit
{
    namespace Renderer
    {
        class Object3d;
    }
    
    class PipelineDescriptor;
    class Camera;
    
    enum class GraphicsAPI
    {
        Metal,
        Vulkan,
        OpenGL,
        DirectX
    };
    
    struct BufferDesc;
    
    struct SamplerDesc
    {};

    struct RenderPassDescriptor
    {
        TextureId depthAttachment;
    };
    
    struct CommandBufferStats
    {
        std::string name;
        double startTimeGPU{ 0.0 };
        double endTimeGPU{ 0.0 };
        double startTimeCPU{ 0.0 };
        double endTimeCPU{ 0.0 };
    };
    
    struct RendererStats
    {
        std::vector<CommandBufferStats> cmdBufStats;
    };
    
    class IRendererStatsProvider
    {
    public:
        virtual ~IRendererStatsProvider() = default;
        
        virtual const RendererStats GetStats() const = 0;
    };
    
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;
        
        virtual void* GetNativeHandle() = 0;
        
        /*!
         @brief Returns enumerated type of underlying graphics API.
         */
        [[nodiscard]] virtual GraphicsAPI GetAPI() const = 0;
        
        virtual std::unique_ptr<Renderer::ISwapChain> CreateSwapChain(const uint32_t width, const uint32_t height) = 0;
        
        /*!
         @brief Returns string description of active device.
         */
        [[nodiscard]] virtual std::string GetDeviceDescription() const = 0;
        
        /*!
         @brief Creates new device buffer.
         @param desc Descriptor of buffer properties.
         @return Unique identifier of created resource.
         */
        [[nodiscard]] virtual BufferId CreateBuffer(const BufferDesc& desc) = 0;
        
        /*!
         @brief Creates new pipeline.
         @param desc Descriptor of pipeline properties.
         @return Unique identifier of created resource.
         */        
        [[nodiscard]] virtual PipelineId CreatePipeline(const PipelineDescriptor& desc) = 0;
        
        /*!
         @brief Creates new texture.
         @param desc Descriptor of pipeline properties.
         @return Unique identifier of created resource.
         */
        [[nodiscard]] virtual TextureId CreateTexture2D(const uint32_t width, const uint32_t height, const void* data) = 0;

        /*!
         @brief Creates new texture.
         @param desc Descriptor of pipeline properties.
         @return Unique identifier of created resource.
         */
        [[nodiscard]] virtual TextureId CreateTexture2D(const TextureDescriptor& desc) = 0;
        
        /*!
         @brief Creates new texture.
         @param desc Descriptor of pipeline properties.
         @return Unique identifier of created resource.
         */
        [[nodiscard]] virtual TextureId CreateTexture2D(const uint32_t width, const uint32_t height) = 0;
        
        [[nodiscard]] virtual SamplerId CreateSampler(const SamplerDesc& desc) = 0;
        
        
        virtual void UpdateTexture2D(const uint32_t width, const uint32_t height, const void* data) = 0;
        
        virtual void UpdateBuffer(BufferId bufferId, void* data, uint32_t size) = 0;
        
        template<typename T>
        void UpdateBuffer(BufferId bufferId, const T& data)
        {
            UpdateBuffer(bufferId, (void*)(&data), sizeof(T));
        }
        
        
        virtual void RenderObject3d(const Renderer::Object3d& object, const PipelineId& effect, const CmdList* cmdList = nullptr) = 0;
        virtual void RenderObject3dInstanced(const Renderer::Object3d& object, const PipelineId& effect, uint32_t instanceCount) = 0;
        
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void BeginScene(const Camera& camera) = 0;
        virtual void EndScene() = 0;
        
        virtual void BeginRenderPass(Renderer::ISwapChain& swapChain, const char* name = "") = 0;
        virtual void BeginRenderPass(const RenderPassDescriptor& desc, const char* name = "") = 0;
        virtual void EndRenderPass() = 0;
        
        virtual void CopyBuffer(BufferId srcBuffId, uint32_t srcIndex, BufferId dstBuffId, uint32_t dstIndex, uint32_t size) = 0;

        virtual void ReleaseBuffer(BufferId bufferId) = 0;
        virtual void ReleaseTexture(TextureId textureId) = 0;
        virtual void ReleaseSampler(SamplerId samplerId) = 0;
        virtual void ReleasePipeline(PipelineId pipelineId) = 0;      
    };
}

namespace std
{
    template <>
    struct hash<Summit::PipelineId>
    {
        size_t operator()(const Summit::PipelineId& key) const
        {
            return key.id;
        }
    };
}
