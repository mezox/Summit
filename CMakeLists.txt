cmake_minimum_required(VERSION 3.13)
project(Summit C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(BUILD_SHARED_LIBS 1)

# Enable project groupping in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Library output path")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Executable output path")

macro(ide_source_files_group)
    foreach(source_file ${ARGN})
        string(REGEX REPLACE "/?[^/]+$" "" folder ${source_file})
        set(folder "${_target_folder}/${folder}")
        string(REPLACE "/" "\\" folder ${folder})

        source_group(${folder} FILES ${source_file})
    endforeach()
endmacro()

macro(append_compile_flags _flags)
	foreach(source_file ${ARGN})
		set_property(SOURCE ${source_file} APPEND_STRING PROPERTY COMPILE_FLAGS " ${_flags}")
	endforeach()
endmacro(append_compile_flags)

macro(set_objective_c_compilation)
    foreach(source ${ARGN})
        append_compile_flags("-x objective-c" ${source})
    endforeach()
endmacro()

macro(set_objective_cxx_compilation)
    foreach(source ${ARGN})
        append_compile_flags("-x objective-c" ${source})
    endforeach()
endmacro()

macro(set_precompiled_header_to_target _target _precompiled_header _precompiled_source)
    set_property(TARGET ${_target} PROPERTY XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES)
    set_property(TARGET ${_target} PROPERTY XCODE_ATTRIBUTE_GCC_PREFIX_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/${_precompiled_header}")
endmacro(set_precompiled_header_to_target)

set(SHARED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Public)

add_subdirectory(External)
add_subdirectory(MetalCpp)
add_subdirectory(MetalRenderer)
add_subdirectory(Renderer)
add_subdirectory(SummitAppKit)
#add_subdirectory(OpenGLRenderer)
add_subdirectory(SummitDemo)
