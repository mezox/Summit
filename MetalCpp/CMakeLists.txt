cmake_minimum_required(VERSION 3.13.0)

project(MetalCpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_METAL_CPP_EXPORT")
endif()

# Platform agnostic dependencies
set(EXTERNAL_DEPENDENCIES
    "-framework Cocoa -framework Metal"
)

set(DEPENDENCIES
)

# Include directories
include_directories(
	"Public"
	"Private"
)

set(PUBLIC_SOURCES
    Public/MetalCpp/MetalCppModule.h
    Public/MetalCpp/MetalWrapperCommon.h
    Public/MetalCpp/MetalDeviceResource.h
    Public/MetalCpp/MetalResource.h
    Public/MetalCpp/MetalCommandQueue.h
    Public/MetalCpp/MetalCommandBuffer.h
    Public/MetalCpp/MetalCommandEncoder.h
    Public/MetalCpp/MetalBuffer.h
    Public/MetalCpp/MetalVertexDescriptor.h
    Public/MetalCpp/MetalTexture.h
    Public/MetalCpp/MetalSampler.h
    Public/MetalCpp/MetalPipeline.h
    Public/MetalCpp/MetalRenderPass.h
    Public/MetalCpp/MetalDevice.h
    Public/MetalCpp/MetalDepthStencil.h
    Public/MetalCpp/MetalTypes.h
    Public/MetalCpp/MetalHeap.h
    Public/MetalCpp/MetalDrawable.h

    # MacOS specific
    Public/MetalCpp/MacOS/Device.hpp
)

set(PRIVATE_SOURCES
    Private/MetalResourceInternal.h
    Private/MetalWrapperCommon.mm
    Private/MetalCommandQueue.mm
    Private/MetalCommandBuffer.mm
    Private/MetalCommandEncoder.mm
    Private/MetalBuffer.mm
    Private/MetalVertexDescriptor.mm
    Private/MetalTexture.mm
    Private/MetalSampler.mm
    Private/MetalPipeline.mm
    Private/MetalRenderPass.mm
    Private/MetalDevice.mm
    Private/MetalTypeConverter.h
    Private/MetalTypeConverter.mm
    Private/TypeConverter.h
    Private/MetalDepthStencil.mm
    Private/MetalHeap.mm
    Private/MetalDrawable.mm
    Private/pch.h
    Private/pch.mm

    Private/MacOS/Device.mm
)

#set_objective_cxx_compilation(Private/pch.cpp)

add_library(${PROJECT_NAME}
	${PUBLIC_SOURCES}
	${PRIVATE_SOURCES}
)

set_precompiled_header_to_target(${PROJECT_NAME} Private/pch.h Private/pch.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE "Public")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES} ${EXTERNAL_DEPENDENCIES})

ide_source_files_group( ${PUBLIC_SOURCES}
                        ${PRIVATE_SOURCES}
)
