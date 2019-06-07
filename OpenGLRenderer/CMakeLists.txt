cmake_minimum_required(VERSION 3.13.0)

project(OpenGLRenderer)

# Module export definition
if (${BUILD_SHARED_LIBS})
	add_definitions("-D_OPENGLRENDERER_EXPORT")
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
	Private/OpenGLRenderer.h
	Private/OpenGLRenderer.mm
)

set(PUBLIC_SOURCES
	Public/OpenGLRenderer/OpenGLRendererBase.h
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
