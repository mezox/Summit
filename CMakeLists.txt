cmake_minimum_required(VERSION 3.3)
project(Summit Swift C CXX)

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

set_objective_c_compilation(main.cpp)

add_executable(${PROJECT_NAME}
	main.cpp
	ObjCMain.m
	SwiftMain.swift
)

set_property(TARGET ${PROJECT_NAME} PROPERTY XCODE_ATTRIBUTE_SWIFT_OBJC_BRIDGING_HEADER "BridgeHeader.h")
#target_compile_options(${PROJECT_NAME} PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:-Werror=objc-method-access>")
