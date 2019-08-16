cmake_minimum_required(VERSION 3.13.0)

project(SummitAppKit)

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_SUMMIT_APP_KIT_EXPORT")
endif()

# Platform agnostic dependencies
set(EXTERNAL_DEPENDENCIES
	"-framework AppKit"
	"-framework Foundation"
	"-framework QuartzCore"
    imgui
    glfw
)

set(DEPENDENCIES
    Renderer
)

# Include directories
include_directories(
	"Public"
	"Private"
)

set(PUBLIC_SOURCES
	Public/SummitAppKit/Window.h
    Public/SummitAppKit/Application.h
    Public/SummitAppKit/Gui.h
    Public/SummitAppKit/Input.h
)

set(PRIVATE_SOURCES
	Private/Window.mm
	Private/View.mm
	Private/macOS/SAWindow.h
	Private/macOS/SAWindow.mm
    Private/macOS/Input.cpp
    Private/Application.cpp
    Private/Gui.cpp
    Private/GuiCallback.h
    Private/GuiCallback.cpp
)

add_library(${PROJECT_NAME}
	${PUBLIC_SOURCES}
	${PRIVATE_SOURCES}
)

#set_objective_cxx_compilation(Private/pch.cpp)

#set_precompiled_header_to_target(${PROJECT_NAME} Private/pch.h Private/pch.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE "Public")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES} ${EXTERNAL_DEPENDENCIES})

ide_source_files_group( ${PUBLIC_SOURCES}
                        ${PRIVATE_SOURCES}
)
