#pragma once

#include "MetalCppModule.h"
#include "MetalCommandQueue.h"

#include <string>
#include <cstdint>

namespace MTL
{
    class Buffer;
    class Texture;
    class Library;
    class SamplerState;
    class RenderPipelineState;
    class RenderPipelineDescriptor;
    class TextureDescriptor;
    class SamplerDescriptor;
    class DepthStencilDescriptor;
    class DepthStencilState;
    class CommandQueue;
    class Heap;
    class HeapDescriptor;
    
    enum class ResourceOptions;
    
    /*!
     @brief Represent a memory size and alignment in bytes.
     */
    struct SizeAndAlign
    {
        uint32_t size;
        uint32_t align;
    };
    
    class METAL_CPP_API MetalDevice : public NSHandleWrapper
    {
    public:
        explicit MetalDevice(void* handle);
        
        [[nodiscard]] std::string GetDeviceName() const noexcept;
        [[nodiscard]] CommandQueue NewCommandQueue(const uint32_t maxCmdBuffers = 1);
        
        /*!
         @abstract Create a new heap with the given descriptor.
         */
        [[nodiscard]] Heap NewHeap(const HeapDescriptor& descriptor) const noexcept;
        
        /*!
         @abstract The current size in bytes of all resources allocated by this device
         */
        [[nodiscard]] uint32_t GetCurrentAllocatedSize() const noexcept;
        
        /*!
         @abstract Returns the default library for the main bundle.
         */
        [[nodiscard]] Library CreateNewDefaultLibrary() const noexcept;
        
        /*!
         @abstract Load a MTLLibrary from source.
         */
        [[nodiscard]] Library NewLibraryWithSource(const std::string& source) const;
        
        /*!
         @abstract Create and compile a new MTLRenderPipelineState object synchronously.
         */
        [[nodiscard]] RenderPipelineState NewRenderPipelineState(const RenderPipelineDescriptor& descriptor) const;
        
        /*!
         @brief Create a buffer by allocating new memory and specifing the initial contents to be copied into it.
         */
        [[nodiscard]] Buffer NewBuffer(const void *pointer, const uint32_t length, ResourceOptions options) const;
        
        /*!
         @brief Create a buffer by allocating new memory.
         */
        [[nodiscard]] Buffer NewBuffer(const uint32_t length, ResourceOptions options) const;
        
        /*!
         @brief Create a depth/stencil test state object.
         */
        [[nodiscard]] DepthStencilState NewDepthStencilState(const DepthStencilDescriptor& desc) const;
        
        /*!
         @abstract Allocate a new texture with privately owned storage.
         */
        [[nodiscard]] Texture NewTexture(const MTL::TextureDescriptor& descriptor) const;
        
        /*!
         @abstract Create a new sampler.
         */
        [[nodiscard]] SamplerState NewSamplerState(const SamplerDescriptor& descriptor) const;
    };
    
    /*!
     @brief Returns a reference to the preferred system default Metal device.
     @discussion On Mac OS X systems that support automatic graphics switching, calling
     this API to get a Metal device will cause the system to switch to the high power
     GPU.  On other systems that support more than one GPU it will return the GPU that
     is associated with the main display.
     */
    MetalDevice CreateSystemDefaultDevice();
}

#include "MacOS/Device.hpp"
