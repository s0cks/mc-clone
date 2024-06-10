# option(BUILD_STATIC_LIBS "Build the static library" ON)
# option(BUILD_SHARED_LIBS "Build the shared library" ON)
# option(BUILD_TESTS "Build test programs" OFF)
option(VMEMORY_PROFILING "Enable video memory profiling" ON)

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

add_compile_definitions(MCC_GLFW)