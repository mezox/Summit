cmake_minimum_required(VERSION 3.13.0)

project(OpenGLRenderer)

add_definitions("-DSUMMIT_GRAPHICS_API_GL")

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
	${SHARED_INCLUDE_DIR}
)

# platform agnostic source files
set(PRIVATE_SOURCES
	# Private
    Private/OpenGLRendererImpl.swift
	${SHARED_INCLUDE_DIR}/Renderer/Renderer.swift
    ${SHARED_INCLUDE_DIR}/Renderer/Renderer.swift
    ${SHARED_INCLUDE_DIR}/Renderer/RendererImpl.h
    ${SHARED_INCLUDE_DIR}/Renderer/RendererImpl.mm
    ${SHARED_INCLUDE_DIR}/Renderer/RendererWrapper.h
    ${SHARED_INCLUDE_DIR}/Renderer/RendererWrapper.m
    ${SHARED_INCLUDE_DIR}/Renderer/RendererBase.h
)

set(PUBLIC_SOURCES
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
