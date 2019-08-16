#pragma once

#include "TypeConverter.h"

#import <Foundation/NSObjCRuntime.h>

// Forward declarations of C++ types
namespace MTL
{
    enum class TextureUsage;
    enum class PrimitiveType;
    enum class IndexType;
    enum class VertexFormat;
    enum class VertexStepFunction;
    enum class SamplerMinMagFilter;
    enum class SamplerMipFilter;
    enum class BlendFactor;
    enum class BlendOperation;
    enum class Winding;
    enum class CullMode;
    enum class CompareFunction;
    enum class PixelFormat;
    enum class StorageMode;
    enum class CPUCacheMode;
    enum class LoadAction;
    enum class StoreAction;
    enum class ResourceOptions;
    enum class PurgeableState;
    enum class FunctionType;
    enum class ResourceUsage;
}

// Forward declarations of Objective-C types
typedef NS_ENUM(NSUInteger, MTLPrimitiveType);
typedef NS_ENUM(NSUInteger, MTLIndexType);
typedef NS_ENUM(NSUInteger, MTLVertexFormat);
typedef NS_ENUM(NSUInteger, MTLVertexStepFunction);
typedef NS_ENUM(NSUInteger, MTLSamplerMinMagFilter);
typedef NS_ENUM(NSUInteger, MTLSamplerMipFilter);
typedef NS_ENUM(NSUInteger, MTLSamplerAddressMode);
typedef NS_ENUM(NSUInteger, MTLBlendFactor);
typedef NS_ENUM(NSUInteger, MTLBlendOperation);
typedef NS_ENUM(NSUInteger, MTLCullMode);
typedef NS_ENUM(NSUInteger, MTLWinding);
typedef NS_ENUM(NSUInteger, MTLCompareFunction);
typedef NS_ENUM(NSUInteger, MTLPixelFormat);
typedef NS_ENUM(NSUInteger, MTLStorageMode);
typedef NS_ENUM(NSUInteger, MTLCacheMode);
typedef NS_ENUM(NSUInteger, MTLLoadAction);
typedef NS_ENUM(NSUInteger, MTLStoreAction);
typedef NS_ENUM(NSUInteger, MTLPurgeableState);
typedef NS_ENUM(NSUInteger, MTLFunctionType);

// Use unsigned long for NS_OPTIONS types as these can't be forward declared
using MTLTextureUsage = unsigned long;
using MTLResourceOptions = unsigned long;
using MTLResourceUsage = unsigned long;

template<> struct TypeLinkerAuto<MTL::TextureUsage>         : public TypeLinkerTempl<MTL::TextureUsage, MTLTextureUsage> {};
template<> struct TypeLinkerAuto<MTL::PrimitiveType>        : public TypeLinkerTempl<MTL::PrimitiveType, MTLPrimitiveType> {};
template<> struct TypeLinkerAuto<MTL::IndexType>            : public TypeLinkerTempl<MTL::IndexType, MTLIndexType> {};

// VertexFormat conversion
template<> struct TypeLinkerAuto<MTL::VertexFormat>         : public TypeLinkerTempl<MTL::VertexFormat, MTLVertexFormat> {};
template<> struct TypeLinkerAuto<MTLVertexFormat>           : public TypeLinkerTempl<MTLVertexFormat, MTL::VertexFormat> {};

template<> struct TypeLinkerAuto<MTL::VertexStepFunction>   : public TypeLinkerTempl<MTL::VertexStepFunction, MTLVertexStepFunction> {};
template<> struct TypeLinkerAuto<MTLVertexStepFunction>     : public TypeLinkerTempl<MTLVertexStepFunction, MTL::VertexStepFunction> {};

// MinMagFilter conversion: CPP -> OBJC
template<> struct TypeLinkerAuto<MTL::SamplerMinMagFilter>  : public TypeLinkerTempl<MTL::SamplerMinMagFilter, MTLSamplerMinMagFilter> {};

// MipFilter conversion: CPP -> OBJC
template<> struct TypeLinkerAuto<MTL::SamplerMipFilter>     : public TypeLinkerTempl<MTL::SamplerMipFilter, MTLSamplerMipFilter> {};

template<> struct TypeLinkerAuto<MTL::BlendFactor>          : public TypeLinkerTempl<MTL::BlendFactor, MTLBlendFactor> {};
template<> struct TypeLinkerAuto<MTL::BlendOperation>       : public TypeLinkerTempl<MTL::BlendOperation, MTLBlendOperation> {};

template<> struct TypeLinkerAuto<MTL::Winding>              : public TypeLinkerTempl<MTL::Winding, MTLWinding> {};
template<> struct TypeLinkerAuto<MTL::CullMode>             : public TypeLinkerTempl<MTL::CullMode, MTLCullMode> {};

template<> struct TypeLinkerAuto<MTL::CompareFunction>      : public TypeLinkerTempl<MTL::CompareFunction, MTLCompareFunction> {};

template<> struct TypeLinkerAuto<MTL::PixelFormat>          : public TypeLinkerTempl<MTL::PixelFormat, MTLPixelFormat> {};

// C++ Storage mode <-> Obj-C StorageMode
template<> struct TypeLinkerAuto<MTL::StorageMode>          : public TypeLinkerTempl<MTL::StorageMode, MTLStorageMode> {};
template<> struct TypeLinkerAuto<MTLStorageMode>            : public TypeLinkerTempl<MTLStorageMode, MTL::StorageMode> {};

// C++ Cpu cache mode <-> Obj-C Cache mode
template<> struct TypeLinkerAuto<MTL::CPUCacheMode>         : public TypeLinkerTempl<MTL::CPUCacheMode, MTLCPUCacheMode> {};
template<> struct TypeLinkerAuto<MTLCPUCacheMode>           : public TypeLinkerTempl<MTLCPUCacheMode, MTL::CPUCacheMode> {};

// C++ Load action -> Obj-C LoadAction
template<> struct TypeLinkerAuto<MTL::LoadAction>           : public TypeLinkerTempl<MTL::LoadAction, MTLLoadAction> {};

// C++ Store action -> Obj-C StoreAction
template<> struct TypeLinkerAuto<MTL::StoreAction>          : public TypeLinkerTempl<MTL::StoreAction, MTLStoreAction> {};

// C++ Store action -> Obj-C StoreAction
template<> struct TypeLinkerAuto<MTL::ResourceOptions>      : public TypeLinkerTempl<MTL::ResourceOptions, MTLResourceOptions> {};

// C++ Purgeable state <-> Obj-C Purgeable state
template<> struct TypeLinkerAuto<MTL::PurgeableState>       : public TypeLinkerTempl<MTL::PurgeableState, MTLPurgeableState> {};
template<> struct TypeLinkerAuto<MTLPurgeableState>         : public TypeLinkerTempl<MTLPurgeableState, MTL::PurgeableState> {};

// C++ Function type <- Obj-C Function type
template<> struct TypeLinkerAuto<MTLFunctionType>         : public TypeLinkerTempl<MTLFunctionType, MTL::FunctionType> {};

// C++ Resource usage -> Obj-C Resource usage
template<> struct TypeLinkerAuto<MTL::ResourceUsage>       : public TypeLinkerTempl<MTL::ResourceUsage, MTLResourceUsage> {};
