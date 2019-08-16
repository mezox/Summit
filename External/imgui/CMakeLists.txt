cmake_minimum_required(VERSION 3.6.0)

project(imgui)

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_IMGUI_EXPORT")
endif()

# Include directories
include_directories(
	"Public"
	"Private"
)

# Platform agnostic dependencies
set(EXTERNAL_DEPENDENCIES
)

set(DEPENDENCIES
)

# platform agnostic source files
set(PRIVATE_SOURCES
	Private/imgui_demo.cpp
	Private/imgui_draw.cpp
	Private/imgui_internal.h
	Private/imgui_widgets.cpp
	Private/imgui.cpp
	Private/imstb_rectpack.h
	Private/imstb_textedit.h
	Private/imstb_truetype.h
)

set(PUBLIC_SOURCES
    Public/imgui/imconfig.h
    Public/imgui/imgui.h
)

add_library(${PROJECT_NAME}
	${PUBLIC_SOURCES}
	${PRIVATE_SOURCES}
)

target_include_directories(${PROJECT_NAME} INTERFACE "Public")
target_link_libraries(${PROJECT_NAME} ${DEPENDENCIES} ${EXTERNAL_DEPENDENCIES})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "${IDE_PROJECT_FOLDER}")
