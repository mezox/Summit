cmake_minimum_required(VERSION 3.13)

project(Application C CXX)

include_directories(${SHARED_INCLUDE_DIR})

# platform agnostic source files
set(APPLICATION_SOURCES
	main.mm
    AppDelegate.h
    AppDelegate.mm
    Resources/Shaders.metal
    Cube.h
    Cube.cpp
    Dragon.h
    Chalet.cpp
    Chalet.h
    ObjLoader.h
    ObjLoader.cpp
    Dragon.cpp
    tiny_obj_loader.h
)

set(APPLICATION_RESOURCES
    Resources/Shaders.metal
    Resources/ImGui.metal
    Resources/texture.jpg
    Resources/chalet.jpg
    Resources/ShadowPass.metal
    Resources/Dragon.metal
    Resources/Chalet.metal
    Resources/Temple.obj
    Resources/Temple.mtl
    Resources/StructureBaseColor.png
    Resources/FoliageBaseColor.png
)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${APPLICATION_SOURCES}
    ${APPLICATION_RESOURCES}
)

target_link_libraries(${PROJECT_NAME} "-framework AppKit -framework Metal -framework MetalKit" MetalRenderer SummitAppKit Renderer)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    RESOURCE "${APPLICATION_RESOURCES}"
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info.plist
)

set_source_files_properties(${APPLICATION_RESOURCES} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
)

