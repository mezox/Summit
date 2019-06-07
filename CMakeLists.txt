cmake_minimum_required(VERSION 3.13)
project(Summit C CXX Swift)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

macro(append_compile_flags _flags)
	foreach(source_file ${ARGN})
		set_property(SOURCE ${source_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${_flags}")
	endforeach()
endmacro(append_compile_flags)

macro(set_objective_c_compilation)
    foreach(source ${ARGN})
        append_compile_flags("-x objective-c" ${source})
        message(STATUS "Set Objective-C compilation for ${source}")
    endforeach()
endmacro()

macro(set_objective_cxx_compilation)
    foreach(source ${ARGN})
        append_compile_flags("-x objective-c" ${source})
        message(STATUS "Set Objective-C compilation for ${source}")
    endforeach()
endmacro()

#set_objective_c_compilation(main.cpp)
#set_objective_cxx_compilation(Private/Metal/MetalRenderer.mm)

include_directories("Public")
include_directories("Private")

set(METAL_SOURCES
	# Public
	Public/Renderer/Renderer.h
	Public/Renderer/MetalRenderer.swift

	# Private
	Private/Metal/MetalRenderer.h
	Private/Metal/MetalRenderer.cpp
	Private/Metal/MetalRendererImpl.swift
	Private/Metal/MetalRendererWrapper.h
	Private/Metal/MetalRendererWrapper.m
	Private/Metal/BridgeHeader.h
)

add_executable(${PROJECT_NAME}
	main.cpp
	${METAL_SOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "Private/Metal/BridgeHeader.h")
#target_compile_options(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-Werror=objc-method-access>")
