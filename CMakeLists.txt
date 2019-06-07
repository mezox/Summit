cmake_minimum_required(VERSION 3.13)
project(Summit C CXX Swift)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS 1)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Library output path")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Bin/${CMAKE_BUILD_TYPE} CACHE PATH "Executable output path")

# Enable project groupping in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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
        message(STATUS "Set Objective-C compilation for ${source}")
    endforeach()
endmacro()

macro(set_objective_cxx_compilation)
    foreach(source ${ARGN})
        append_compile_flags("-x objective-c" ${source})
        message(STATUS "Set Objective-C compilation for ${source}")
    endforeach()
endmacro()

set(SHARED_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/Public)

add_subdirectory(MetalRenderer)
add_subdirectory(OpenGLRenderer)
add_subdirectory(SummitDemo)
