cmake_minimum_required(VERSION 3.5)
project(minecraft)


# option(BUILD_STATIC_LIBS "Build the static library" ON)
# option(BUILD_SHARED_LIBS "Build the shared library" ON)
# option(BUILD_TESTS "Build test programs" OFF)
option(VMEMORY_PROFILING "Enable video memory profiling" ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

find_package(Threads REQUIRED)
find_package(Glog REQUIRED)
find_package(gflags REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)
find_package(libuv REQUIRED)
find_package(GLM REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(JPEG REQUIRED)
find_package(rx REQUIRED)
find_package(nk REQUIRED)
find_package(leveldb REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(utf8 REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Compiling w/ debug mode.")
    add_compile_definitions(MCC_DEBUG)
else()
    message(STATUS "Compiling w/o debug mode.")
endif()

if(VMEMORY_PROFILING)
    message(STATUS "Compiling w/ video memory profiling.")
    add_compile_definitions(MCC_VMEMPROF)
endif()

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

include_directories(${RX_INCLUDE_DIRS})
include_directories(${RAPIDJSON_INCLUDE_DIRS})
include_directories(${NK_INCLUDE_DIRS})

add_subdirectory(Tests)
add_subdirectory(Sources)
add_subdirectory(Benchmarks)

add_subdirectory(lib/units)
add_subdirectory(lib/semver)