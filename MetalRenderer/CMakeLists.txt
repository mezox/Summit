cmake_minimum_required(VERSION 3.13.0)

project(MetalRenderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_METAL_RENDERER_EXPORT")
endif()

# Platform agnostic dependencies
set(EXTERNAL_DEPENDENCIES
    "-framework AppKit"
    "-framework Foundation"
    "-framework QuartzCore"
)

set(DEPENDENCIES
    MetalCpp
    Renderer
)

# Include directories
include_directories(
	"Public"
	"Private"
)

set(PUBLIC_SOURCES
    Public/MetalRenderer/MetalRendererModule.h
    Public/MetalRenderer/MetalRenderer.h
    Public/MetalRenderer/MetalRenderPass.h
)

set(PRIVATE_SOURCES
    Private/MetalRenderer.mm
    Private/MetalSwapChain.mm
    Private/MetalSwapChain.h
    Private/MetalRenderPass.cpp
    Private/MacOS/SAView.h
    Private/MacOS/SAView.mm
    Private/RendererToMetalConverter.h
    Private/RendererToMetalConverter.cpp
    Private/DeviceResourceIdImpl.h
    Private/DeviceResourceIdImpl.cpp

    Private/pch.h
    Private/pch.mm
)

add_library(${PROJECT_NAME}
	${PUBLIC_SOURCES}
	${PRIVATE_SOURCES}
)

set_objective_cxx_compilation(Private/pch.cpp)

set_precompiled_header_to_target(${PROJECT_NAME} Private/pch.h Private/pch.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE "Public")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES} ${EXTERNAL_DEPENDENCIES})

ide_source_files_group( ${PUBLIC_SOURCES}
                        ${PRIVATE_SOURCES}
)
