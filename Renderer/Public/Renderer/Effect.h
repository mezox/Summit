#pragma once

#include "Renderer.h"

#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

namespace Summit
{
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
        Texture,
        Sampler
    };
    
    enum class ResourceUsage
    {
        Read,
        Write,
        Sample
    };
    
    enum class Format
    {
        Invalid,
        Float,
        Float2,
        Float3,
        Float4,
        UChar4
    };
    
    struct UniformDescriptor
    {
        UniformType type{ UniformType::Undefined };
        ModuleStage stage{ ModuleStage::Undefined };
        uint32_t id{ 0 };
    };
    
    using UniformBindingDesc = std::vector<UniformDescriptor>;
    
    struct ModuleDescriptor
    {
        ModuleStage type;
        std::string moduleEntryPoint;
        std::vector<UniformBindingDesc> bindingDescriptors;
    };
    
    struct ConstantRangeDescriptor
    {
        ModuleStage stage{ ModuleStage::Undefined };
        uint32_t offset{ 0 };
        uint32_t size{ 0 };
    };
    
    class PipelineDescriptor final
    {
    public:
        void AddModule(ModuleStage type, const std::string& entryPoint);
        void AddAttribute(Format f, uint32_t binding);
        void AddResource(UniformType type, ModuleStage stage, uint32_t binding, uint32_t resourceId);
        void SetStageConstant(ModuleStage type, void* data, uint32_t binding);
        
        const uint32_t GetAttributeBindingCount() const;
        const std::vector<Format>& GetAttributes(const uint32_t binding) const;
        const std::vector<ModuleDescriptor>& GetModuleDescriptors() const;
        
        // Debug stuff
        void SetPipelineName(std::string name) { mPipelineName = std::move(name); }
        const std::string& GetPipelineName() const noexcept { return mPipelineName; }

    public:
        bool enableBlending{ true };
        
    private:
        std::vector<ModuleDescriptor> mModuleDescriptors;
        std::unordered_map<uint8_t, std::vector<Format>> mAttribBindings;
        std::vector<ConstantRangeDescriptor> mConstantBindings;
        std::string mPipelineName;
    };
    
    class Effect final
    {
    public:
        Effect(const Effect& other) = delete;
        Effect(Effect&& other) = delete;
        Effect& operator=(const Effect& other) = delete;
        Effect& operator=(Effect&& other) = delete;
        
    private:
        PipelineDescriptor mPipelineDescriptor;
        PipelineId mPipelineId;
    };
}
