#include <Renderer/Effect.h>

#include <exception>

namespace Summit
{
    void PipelineDescriptor::AddModule(ModuleStage type, const std::string& entryPoint)
    {
        mModuleDescriptors.push_back({ type, entryPoint });
    }

    void PipelineDescriptor::AddAttribute(const Format f, const uint32_t binding)
    {
        mAttribBindings[binding].push_back(f);
    }

    const std::vector<ModuleDescriptor>& PipelineDescriptor::GetModuleDescriptors() const
    {
        return mModuleDescriptors;
    }

    const uint32_t PipelineDescriptor::GetAttributeBindingCount() const
    {
        return static_cast<uint32_t>(mAttribBindings.size());
    }

    const std::vector<Format>& PipelineDescriptor::GetAttributes(const uint32_t binding) const
    {
        if(mAttribBindings.find(binding) == mAttribBindings.end())
            throw std::runtime_error("Invalid attribute binding key");
        
        return mAttribBindings.at(binding);
    }

    void PipelineDescriptor::SetStageConstant(ModuleStage type, void* data, uint32_t binding)
    {
        
    }
    
    void PipelineDescriptor::AddResource(const UniformType type, const ModuleStage stage, const uint32_t binding, const uint32_t resourceId)
    {
        auto result = std::find_if(mModuleDescriptors.begin(), mModuleDescriptors.end(), [stage](const auto& descriptor){
            return descriptor.type == stage;
        });
        
        if(result == mModuleDescriptors.end())
        {
            throw std::runtime_error("Attempt to assign uniform descriptor to nonexistent pipeline stage");
        }
        
        auto& moduleDescriptor = *result;
        
        if(binding >= moduleDescriptor.bindingDescriptors.capacity())
            moduleDescriptor.bindingDescriptors.resize(binding + 1);
        
        moduleDescriptor.bindingDescriptors[binding].push_back({ type, stage, resourceId });
    }
}
