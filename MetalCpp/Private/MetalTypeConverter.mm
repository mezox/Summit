#include "pch.h"

#include "MetalTypeConverter.h"

#include <MetalCpp/MetalResource.h>
#include <MetalCpp/MetalTexture.h>
#include <MetalCpp/MetalCommandEncoder.h>
#include <MetalCpp/MetalVertexDescriptor.h>
#include <MetalCpp/MetalSampler.h>
#include <MetalCpp/MetalPipeline.h>
#include <MetalCpp/MetalDepthStencil.h>
#include <MetalCpp/MetalRenderPass.h>

template<>
auto TypeLinkerTempl<MTL::TextureUsage, MTLTextureUsage>::operator()(const from_t& usage) -> to_t
{
    switch(usage)
    {
        case MTL::TextureUsage::Unknown:            return MTLTextureUsageUnknown;
        case MTL::TextureUsage::ShaderRead:         return MTLTextureUsageShaderRead;
        case MTL::TextureUsage::ShaderWrite:        return MTLTextureUsageShaderWrite;
        case MTL::TextureUsage::RenderTarget:       return MTLTextureUsageRenderTarget;
        case MTL::TextureUsage::PixelFormatView:    return MTLTextureUsagePixelFormatView;
    }
    
    return MTLTextureUsageUnknown;
}

template<>
auto TypeLinkerTempl<MTL::PrimitiveType, MTLPrimitiveType>::operator()(const from_t& type) -> to_t
{
    switch (type)
    {
        case MTL::PrimitiveType::Point:             return MTLPrimitiveTypePoint;
        case MTL::PrimitiveType::Line:              return MTLPrimitiveTypeLine;
        case MTL::PrimitiveType::LineStrip:         return MTLPrimitiveTypeLineStrip;
        case MTL::PrimitiveType::Triangle:          return MTLPrimitiveTypeTriangle;
        case MTL::PrimitiveType::TriangleStrip:     return MTLPrimitiveTypeTriangleStrip;
    }
    
    return MTLPrimitiveTypeTriangle;
}

template<>
auto TypeLinkerTempl<MTL::IndexType, MTLIndexType>::operator()(const from_t& type) -> to_t
{
    switch (type)
    {
        case MTL::IndexType::UInt16:    return MTLIndexTypeUInt16;
        case MTL::IndexType::UInt32:    return MTLIndexTypeUInt32;
    }
    
    return MTLIndexTypeUInt16;
}

template<>
auto TypeLinkerTempl<MTL::VertexFormat, MTLVertexFormat>::operator()(const from_t& format) -> to_t
{
    switch (format)
    {
        case MTL::VertexFormat::Invalid:     return MTLVertexFormatInvalid;
        case MTL::VertexFormat::UChar2:      return MTLVertexFormatUChar2;
        case MTL::VertexFormat::UChar3:      return MTLVertexFormatUChar3;
        case MTL::VertexFormat::UChar4:      return MTLVertexFormatUChar4;
        case MTL::VertexFormat::Float:       return MTLVertexFormatFloat;
        case MTL::VertexFormat::Float2:      return MTLVertexFormatFloat2;
        case MTL::VertexFormat::Float3:      return MTLVertexFormatFloat3;
        case MTL::VertexFormat::Float4:      return MTLVertexFormatFloat4;
    }
    
    return MTLVertexFormatInvalid;
};

template<>
auto TypeLinkerTempl<MTLVertexFormat, MTL::VertexFormat>::operator()(const from_t& format) -> to_t
{
    switch (format)
    {
        case MTLVertexFormatInvalid: return MTL::VertexFormat::Invalid;
        case MTLVertexFormatUChar2:  return MTL::VertexFormat::UChar2;
        case MTLVertexFormatUChar3:  return MTL::VertexFormat::UChar3;
        case MTLVertexFormatUChar4:  return MTL::VertexFormat::UChar4;
        case MTLVertexFormatFloat:   return MTL::VertexFormat::Float;
        case MTLVertexFormatFloat2:  return MTL::VertexFormat::Float2;
        case MTLVertexFormatFloat3:  return MTL::VertexFormat::Float3;
        case MTLVertexFormatFloat4:  return MTL::VertexFormat::Float4;
    }
    
    return MTL::VertexFormat::Invalid;
};

template<>
auto TypeLinkerTempl<MTL::VertexStepFunction, MTLVertexStepFunction>::operator()(const from_t& stepFunc) -> to_t
{
    switch (stepFunc)
    {
        case MTL::VertexStepFunction::Constant:              return MTLVertexStepFunctionConstant;
        case MTL::VertexStepFunction::PerVertex:             return MTLVertexStepFunctionPerVertex;
        case MTL::VertexStepFunction::PerInstance:           return MTLVertexStepFunctionPerInstance;
        case MTL::VertexStepFunction::PerPatch:              return MTLVertexStepFunctionPerPatch;
        case MTL::VertexStepFunction::PerPatchControlPoint:  return MTLVertexStepFunctionPerPatchControlPoint;
    }
    
    return MTLVertexStepFunctionPerVertex;
}

template<>
auto TypeLinkerTempl<MTLVertexStepFunction, MTL::VertexStepFunction>::operator()(const from_t& stepFunc) -> to_t
{
    switch (stepFunc)
    {
        case MTLVertexStepFunctionConstant:                 return MTL::VertexStepFunction::Constant;
        case MTLVertexStepFunctionPerVertex:                return MTL::VertexStepFunction::PerVertex;
        case MTLVertexStepFunctionPerInstance:              return MTL::VertexStepFunction::PerInstance;
        case MTLVertexStepFunctionPerPatch:                 return MTL::VertexStepFunction::PerPatch;
        case MTLVertexStepFunctionPerPatchControlPoint:     return MTL::VertexStepFunction::PerPatchControlPoint;
    }
    
    return MTL::VertexStepFunction::PerVertex;
}

template<>
auto TypeLinkerTempl<MTL::SamplerMinMagFilter, MTLSamplerMinMagFilter>::operator()(const from_t& filter) -> to_t
{
    switch (filter)
    {
        case MTL::SamplerMinMagFilter::Nearest: return MTLSamplerMinMagFilterNearest;
        case MTL::SamplerMinMagFilter::Linear:  return MTLSamplerMinMagFilterLinear;
    }
    
    return MTLSamplerMinMagFilterNearest;
}

template<>
auto TypeLinkerTempl<MTL::SamplerMipFilter, MTLSamplerMipFilter>::operator()(const from_t& filter) -> to_t
{
    switch (filter)
    {
        case MTL::SamplerMipFilter::NotMipmapped:   return MTLSamplerMipFilterNotMipmapped;
        case MTL::SamplerMipFilter::Nearest:        return MTLSamplerMipFilterNearest;
        case MTL::SamplerMipFilter::Linear:         return MTLSamplerMipFilterLinear;
    }
    
    return MTLSamplerMipFilterNotMipmapped;
}

template<>
auto TypeLinkerTempl<MTL::BlendFactor, MTLBlendFactor>::operator()(const from_t& factor) -> to_t
{
    switch(factor)
    {
        case MTL::BlendFactor::Zero:                        return MTLBlendFactorZero;
        case MTL::BlendFactor::One:                         return MTLBlendFactorOne;
        case MTL::BlendFactor::SourceColor:                 return MTLBlendFactorSourceColor;
        case MTL::BlendFactor::OneMinusSourceColor:         return MTLBlendFactorOneMinusSourceColor;
        case MTL::BlendFactor::SourceAlpha:                 return MTLBlendFactorSourceAlpha;
        case MTL::BlendFactor::OneMinusSourceAlpha:         return MTLBlendFactorOneMinusSourceAlpha;
        case MTL::BlendFactor::DestinationColor:            return MTLBlendFactorDestinationColor;
        case MTL::BlendFactor::OneMinusDestinationColor:    return MTLBlendFactorOneMinusDestinationColor;
        case MTL::BlendFactor::DestinationAlpha:            return MTLBlendFactorDestinationAlpha;
        case MTL::BlendFactor::OneMinusDestinationAlpha:    return MTLBlendFactorOneMinusDestinationAlpha;
        case MTL::BlendFactor::SourceAlphaSaturated:        return MTLBlendFactorSourceAlphaSaturated;
        case MTL::BlendFactor::BlendColor:                  return MTLBlendFactorBlendColor;
        case MTL::BlendFactor::OneMinusBlendColor:          return MTLBlendFactorOneMinusBlendColor;
        case MTL::BlendFactor::BlendAlpha:                  return MTLBlendFactorBlendAlpha;
        case MTL::BlendFactor::OneMinusBlendAlpha:          return MTLBlendFactorOneMinusBlendAlpha;
        case MTL::BlendFactor::Source1Color:                return MTLBlendFactorSource1Color;
        case MTL::BlendFactor::OneMinusSource1Color:        return MTLBlendFactorOneMinusSource1Color;
        case MTL::BlendFactor::Source1Alpha:                return MTLBlendFactorSource1Alpha;
        case MTL::BlendFactor::OneMinusSource1Alpha:        return MTLBlendFactorOneMinusSource1Alpha;
    }
    
    return MTLBlendFactorZero;
};

template<>
auto TypeLinkerTempl<MTL::BlendOperation, MTLBlendOperation>::operator()(const from_t& op) -> to_t
{
    switch(op)
    {
        case MTL::BlendOperation::Add:              return MTLBlendOperationAdd;
        case MTL::BlendOperation::Subtract:         return MTLBlendOperationSubtract;
        case MTL::BlendOperation::ReverseSubtract:  return MTLBlendOperationReverseSubtract;
        case MTL::BlendOperation::Min:              return MTLBlendOperationMin;
        case MTL::BlendOperation::Max:              return MTLBlendOperationMax;
    }
    
    return MTLBlendOperationAdd;
};

template<>
auto TypeLinkerTempl<MTL::Winding, MTLWinding>::operator()(const from_t& winding) -> to_t
{
    switch(winding)
    {
        case MTL::Winding::Clockwise:           return MTLWindingClockwise;
        case MTL::Winding::CounterClockwise:    return MTLWindingCounterClockwise;
    }
    
    return MTLWindingClockwise;
};

template<>
auto TypeLinkerTempl<MTL::CullMode, MTLCullMode>::operator()(const from_t& cm) -> to_t
{
    switch(cm)
    {
        case MTL::CullMode::None:   return MTLCullModeNone;
        case MTL::CullMode::Front:  return MTLCullModeFront;
        case MTL::CullMode::Back:   return MTLCullModeBack;
    }
    
    return MTLCullModeNone;
};

template<>
auto TypeLinkerTempl<MTL::CompareFunction, MTLCompareFunction>::operator()(const from_t& cf) -> to_t
{
    switch(cf)
    {
        case MTL::CompareFunction::Never:           return MTLCompareFunctionNever;
        case MTL::CompareFunction::Less:            return MTLCompareFunctionLess;
        case MTL::CompareFunction::Equal:           return MTLCompareFunctionEqual;
        case MTL::CompareFunction::LessEqual:       return MTLCompareFunctionLessEqual;
        case MTL::CompareFunction::Greater:         return MTLCompareFunctionGreater;
        case MTL::CompareFunction::NotEqual:        return MTLCompareFunctionNotEqual;
        case MTL::CompareFunction::GreaterEqual:    return MTLCompareFunctionGreaterEqual;
        case MTL::CompareFunction::Always:          return MTLCompareFunctionAlways;
    }
    
    return MTLCompareFunctionNever;
};

template<>
auto TypeLinkerTempl<MTL::PixelFormat, MTLPixelFormat>::operator()(const from_t& cf) -> to_t
{
    switch(cf)
    {
        case MTL::PixelFormat::Invalid:           return MTLPixelFormatInvalid;
        case MTL::PixelFormat::RGBA8Unorm:        return MTLPixelFormatRGBA8Unorm;
        case MTL::PixelFormat::Depth32Float:      return MTLPixelFormatDepth32Float;
    }
    
    return MTLPixelFormatInvalid;
};

template<>
auto TypeLinkerTempl<MTL::StorageMode, MTLStorageMode>::operator()(const from_t& sm) -> to_t
{
    switch(sm)
    {
        case MTL::StorageMode::Shared:      return MTLStorageModeShared;
        case MTL::StorageMode::Managed:     return MTLStorageModeManaged;
        case MTL::StorageMode::Private:     return MTLStorageModePrivate;
            
        // TODO: Add some platform checks
        //case MTL::StorageMode::Memoryless:  return MTLStorageModeMemoryless;
    }
    
    return MTLStorageModeShared;
};

template<>
auto TypeLinkerTempl<MTLStorageMode, MTL::StorageMode>::operator()(const from_t& sm) -> to_t
{
    switch(sm)
    {
        case MTLStorageModeShared:  return MTL::StorageMode::Shared;
        case MTLStorageModeManaged: return MTL::StorageMode::Managed;
        case MTLStorageModePrivate: return MTL::StorageMode::Private;
            
            // TODO: Add some platform checks
            //case MTL::StorageMode::Memoryless:  return MTLStorageModeMemoryless;
    }
    
    return MTL::StorageMode::Shared;
};

template<>
auto TypeLinkerTempl<MTL::CPUCacheMode, MTLCPUCacheMode>::operator()(const from_t& cm) -> to_t
{
    switch(cm)
    {
        case MTL::CPUCacheMode::DefaultCache:      return MTLCPUCacheModeDefaultCache;
        case MTL::CPUCacheMode::WriteCombined:     return MTLCPUCacheModeWriteCombined;
    }
    
    return MTLCPUCacheModeDefaultCache;
};

template<>
auto TypeLinkerTempl<MTLCPUCacheMode, MTL::CPUCacheMode>::operator()(const from_t& cm) -> to_t
{
    switch(cm)
    {
        case MTLCPUCacheModeDefaultCache:      return MTL::CPUCacheMode::DefaultCache;
        case MTLCPUCacheModeWriteCombined:     return MTL::CPUCacheMode::WriteCombined;
    }
    
    return MTL::CPUCacheMode::DefaultCache;
};

template<>
auto TypeLinkerTempl<MTL::LoadAction, MTLLoadAction>::operator()(const from_t& la) -> to_t
{
    switch(la)
    {
        case MTL::LoadAction::DontCare:      return MTLLoadActionDontCare;
        case MTL::LoadAction::Load:          return MTLLoadActionLoad;
        case MTL::LoadAction::Clear:         return MTLLoadActionClear;
    }
    
    return MTLLoadActionDontCare;
};

template<>
auto TypeLinkerTempl<MTL::StoreAction, MTLStoreAction>::operator()(const from_t& sa) -> to_t
{
    switch(sa)
    {
        case MTL::StoreAction::DontCare:                        return MTLStoreActionDontCare;
        case MTL::StoreAction::Store:                           return MTLStoreActionStore;
        case MTL::StoreAction::MultisampleResolve:              return MTLStoreActionMultisampleResolve;
        case MTL::StoreAction::StoreAndMultisampleResolve:      return MTLStoreActionStoreAndMultisampleResolve;
        case MTL::StoreAction::Unknown:                         return MTLStoreActionUnknown;
        case MTL::StoreAction::CustomSampleDepthStore:          return MTLStoreActionCustomSampleDepthStore;
    }
    
    return MTLStoreActionDontCare;
};

template<>
auto TypeLinkerTempl<MTL::ResourceOptions, MTLResourceOptions>::operator()(const from_t& ro) -> to_t
{
    /*
     Note:  this has to be handled via multiple ifs, because MTL::ResourceOptions contain enum values
            with same values, which would not compile with switch statement
     */
    
    if(ro == MTL::ResourceOptions::CPUCacheModeDefaultCache)        return MTLResourceOptionCPUCacheModeDefault;
    if(ro == MTL::ResourceOptions::CPUCacheModeWriteCombined)       return MTLResourceOptionCPUCacheModeWriteCombined;
    if(ro == MTL::ResourceOptions::StorageModeShared)               return MTLResourceStorageModeShared;
    if(ro == MTL::ResourceOptions::StorageModeManaged)              return MTLResourceStorageModeManaged;
    if(ro == MTL::ResourceOptions::StorageModePrivate)              return MTLResourceStorageModePrivate;
    
    // Not available on macOS
    //case MTL::ResourceOptions::StorageModeMemoryless:           return MTLResourceStorageModeMemoryless;
    
    if(ro == MTL::ResourceOptions::HazardTrackingModeUntracked)     return MTLResourceHazardTrackingModeUntracked;
    //case MTL::ResourceOptions::OptionCPUCacheModeDefault:       return MTLResourceCPUCacheModeDefaultCache;
    //case MTL::ResourceOptions::StorageModeMemoryless:           return MTLResourceOptionCPUCacheModeWriteCombined;
    
    return MTLResourceOptionCPUCacheModeDefault;
};

template<>
auto TypeLinkerTempl<MTL::PurgeableState, MTLPurgeableState>::operator()(const from_t& ps) -> to_t
{
    switch(ps)
    {
        case MTL::PurgeableState::KeepCurrent:                     return MTLPurgeableStateKeepCurrent;
        case MTL::PurgeableState::NonVolatile:                     return MTLPurgeableStateNonVolatile;
        case MTL::PurgeableState::Volatile:                        return MTLPurgeableStateVolatile;
        case MTL::PurgeableState::Empty:                           return MTLPurgeableStateEmpty;
    }
    
    throw std::runtime_error("Unimplemented purgeable state conversion");
    
    return MTLPurgeableStateKeepCurrent;
};

template<>
auto TypeLinkerTempl<MTLPurgeableState, MTL::PurgeableState>::operator()(const from_t& ps) -> to_t
{
    switch(ps)
    {
        case MTLPurgeableStateKeepCurrent:                     return MTL::PurgeableState::KeepCurrent;
        case MTLPurgeableStateNonVolatile:                     return MTL::PurgeableState::NonVolatile;
        case MTLPurgeableStateVolatile:                        return MTL::PurgeableState::Volatile;
        case MTLPurgeableStateEmpty:                           return MTL::PurgeableState::Empty;
    }
    
    throw std::runtime_error("Unimplemented purgeable state conversion");
    
    return MTL::PurgeableState::KeepCurrent;
};

template<>
auto TypeLinkerTempl<MTLFunctionType, MTL::FunctionType>::operator()(const from_t& ft) -> to_t
{
    switch(ft)
    {
        case MTLFunctionTypeVertex:     return MTL::FunctionType::Vertex;
        case MTLFunctionTypeFragment:   return MTL::FunctionType::Fragment;
        case MTLFunctionTypeKernel:     return MTL::FunctionType::Kernel;
    }
    
    throw std::runtime_error("Unimplemented function type conversion");
    
    return MTL::FunctionType::Vertex;
};

template<>
auto TypeLinkerTempl<MTL::ResourceUsage, MTLResourceUsage>::operator()(const from_t& ru) -> to_t
{
    switch(ru)
    {
        case MTL::ResourceUsage::Read:                     return MTLResourceUsageRead;
        case MTL::ResourceUsage::Write:                    return MTLResourceUsageWrite;
        case MTL::ResourceUsage::Sample:                   return MTLResourceUsageSample;
    }
    
    throw std::runtime_error("Unimplemented resource usage conversion");
    
    return MTLResourceUsageRead;
};
