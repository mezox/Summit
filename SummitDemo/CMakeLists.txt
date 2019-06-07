cmake_minimum_required(VERSION 3.13)

project(Application)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -stdlib=libc++")

include_directories(
	"."
	${SHARED_INCLUDE_DIR}
)

# platform agnostic source files
set(APPLICATION_SOURCES
	main.cpp
)

add_executable(${PROJECT_NAME} ${APPLICATION_SOURCES})
