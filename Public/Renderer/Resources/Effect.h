#pragma once

#include "RendererBase.h"
#include "SharedDeviceTypes.h"
#include "DeviceObject.h"

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace Renderer
{
    class Buffer;
    class Attachable;
    
    enum class ModuleStage
    {
        Undefined,
        Vertex,
        Fragment
    };
    
    enum class UniformType
    {
        Undefined,
        Buffer,
        Sampler
    };
    
    class RENDERER_API Effect
    {
        friend class VulkanRenderer;
        
    public:
        struct ModuleDescriptor
        {
            ModuleStage type;
            std::string filePath;
        };
        
        struct UniformDescriptor
        {
            UniformType type{ UniformType::Undefined };
            ModuleStage stage{ ModuleStage::Undefined };
            uint32_t count{ 0 };
        };
        
        struct ConstantRangeDescriptor
        {
            ModuleStage stage{ ModuleStage::Undefined };
            uint32_t offset{ 0 };
            uint32_t size{ 0 };
        };
        
        using UniformBindingDesc = std::vector<UniformDescriptor>;
        
    public:
        Effect() = default;
        
        virtual ~Effect() = default;
        
        Effect(const Effect& other) = delete;
        Effect(Effect&& other) = delete;
        Effect& operator=(const Effect& other) = delete;
        Effect& operator=(Effect&& other) = delete;
        
        void AddModule(ModuleStage type, const std::string& filePath);
        void AddAttributeExplicit(Format f, uint32_t stride, uint32_t location, uint32_t binding, uint32_t offset);
        void AddAttribute(Format f, uint32_t binding);
        
        void AddUniform(UniformType f, ModuleStage stage, uint32_t binding, uint32_t count);
        void AddConstantRange(ModuleStage stage, uint32_t offset, uint32_t size);
        
        void AddUniformBuffer(ModuleStage stage, uint32_t binding, const Buffer& buffer);
        void AddTexture(ModuleStage stage, uint32_t binding, const Attachable& image);
        
        uint8_t GetBindingCount() const;
        const std::vector<Format>& GetBindingDescriptor(uint8_t binding) const;
        const std::vector<ModuleDescriptor>& GetModuleDescriptors() const;
        const std::vector<UniformBindingDesc>& GetUniformBindings() const;
        
    private:
        std::vector<ModuleDescriptor> mModuleDescriptors;
        std::unordered_map<uint8_t, std::vector<Format>> mAttribBindings;
        std::vector<UniformBindingDesc> mUniformBindings;
        std::vector<ConstantRangeDescriptor> mConstantRanges;
        
        std::vector<DeviceObject> mModules;
        std::vector<DeviceObject> mDescriptorSetLayouts;
        std::vector<DeviceObject> mDescriptorSets;
        
        std::vector<const Buffer*> mUniformBuffers;
        std::vector<const Attachable*> mTextures;
    };
}
