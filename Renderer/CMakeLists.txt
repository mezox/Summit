cmake_minimum_required(VERSION 3.13.0)

project(Renderer)

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_RENDERER_EXPORT")
endif()

# Platform agnostic dependencies
set(EXTERNAL_DEPENDENCIES
)

set(DEPENDENCIES
)

# Include directories
include_directories(
	"Public"
	"Private"
)

set(PUBLIC_SOURCES
    Public/Renderer/Object3d.h
    Public/Renderer/RendererResource.h
    Public/Renderer/VertexBuffer.h
    Public/Renderer/Effect.h
    Public/Renderer/Buffer.h
    Public/Renderer/Renderer.h
    Public/Renderer/SwapChain.h
    Public/Renderer/Command.h
    Public/Renderer/CommandBuffer.h
    Public/Renderer/Signal.h
    Public/Renderer/PointerEvent.h
    Public/Renderer/DeviceResourceId.h
    Public/Renderer/Mesh.h
    Public/Renderer/Layer.h
    Public/Renderer/LayerStack.h
    Public/Renderer/Math.h
    Public/Renderer/Matrix4.h
    Public/Renderer/RendererTypes.h
    Public/Renderer/Camera.h
    Public/Renderer/RenderPass.h

    # Math
    Public/Renderer/Math/Math.h

    # Math implementation
    Public/Renderer/Math/detail/defines.h
    Public/Renderer/Math/detail/vector.h
    Public/Renderer/Math/detail/quaternion.h
    Public/Renderer/Math/detail/matrix.h
    Public/Renderer/Math/detail/simd/quaternion_simd.h
    Public/Renderer/Math/detail/simd/vector_simd.h
    Public/Renderer/Math/detail/simd/matrix_simd.h
    Public/Renderer/Math/detail/simd/simd_types.h

    Public/Renderer/Framegraph/ResourceBase.hpp
    Public/Renderer/Framegraph/Resource.hpp
    Public/Renderer/Framegraph/RenderTaskBase.hpp
    Public/Renderer/Framegraph/RenderTask.hpp
    Public/Renderer/Framegraph/RenderTaskBuilder.hpp
    Public/Renderer/Framegraph/Framegraph.hpp

    Public/Renderer/Resources/Texture.hpp
)

# platform agnostic source files
set(PRIVATE_SOURCES
    Private/Object3d.cpp
    Private/RendererResource.cpp
    Private/VertexBuffer.cpp
    Private/Effect.cpp
    Private/CommandBuffer.cpp
    Private/Command.cpp
    Private/CommandImpl.h
    Private/CommandImpl.cpp
    Private/Mesh.cpp
    Private/Layer.cpp
    Private/LayerStack.cpp
    Private/Matrix4.cpp
    Private/Camera.cpp
    Private/RenderPass.cpp

    Private/Resources/Texture.cpp
)

add_library(${PROJECT_NAME}
	${PUBLIC_SOURCES}
	${PRIVATE_SOURCES}
)

target_include_directories(${PROJECT_NAME} INTERFACE "Public")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES} ${EXTERNAL_DEPENDENCIES})

ide_source_files_group( ${PUBLIC_SOURCES}
                        ${PRIVATE_SOURCES}
)

#target_compile_options(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-Werror=objc-method-access>")
