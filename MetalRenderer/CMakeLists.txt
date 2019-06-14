cmake_minimum_required(VERSION 3.13.0)

project(MetalRenderer)

add_definitions("-DSUMMIT_GRAPHICS_API_METAL")

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_METALRENDERER_EXPORT")
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
	${SHARED_INCLUDE_DIR}
)

# platform agnostic source files
set(PRIVATE_SOURCES
	# Private
    Private/MetalRendererImpl.swift
	${SHARED_INCLUDE_DIR}/Renderer/Renderer.swift
    ${SHARED_INCLUDE_DIR}/Renderer/RendererImpl.h
    ${SHARED_INCLUDE_DIR}/Renderer/RendererImpl.mm
    ${SHARED_INCLUDE_DIR}/Renderer/RendererWrapper.h
    ${SHARED_INCLUDE_DIR}/Renderer/RendererWrapper.m
)

set(PUBLIC_SOURCES
	Public/MetalRenderer/MetalRendererBase.h
)

#set_objective_cxx_compilation(Private/MetalRenderer.cpp)

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
