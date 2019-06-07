cmake_minimum_required(VERSION 3.13.0)

project(MetalRenderer)

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
	Private/MetalRenderer.h
	Private/MetalRenderer.mm
    Private/MetalRendererImpl.swift
	Private/MetalRendererWrapper.h
	Private/MetalRendererWrapper.m
	Private/BridgeHeader.h
	${SHARED_INCLUDE_DIR}/Renderer/Renderer.swift
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

#set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "Private/BridgeHeader.h")
#target_compile_options(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-Werror=objc-method-access>")
