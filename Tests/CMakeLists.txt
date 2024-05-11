find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB MCC_SHADER_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.cc")

file(GLOB MCC_PROGRAM_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/program/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/program/*.cc")

file(GLOB MCC_VAO_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vao/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/vao/*.cc")

file(GLOB MCC_TEXTURE_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/texture/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/texture/*.cc")

file(GLOB MCC_IMAGE_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/image/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/image/*.cc")

file(GLOB MCC_URI_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri_*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri_*.cc")

add_executable(${PROJECT_NAME}-tests
  main.cc
  ${MCC_URI_TEST_SOURCES}
  mcc/test_cache.h mcc/test_cache.cc
  mcc/test_rapidjson.h mcc/test_rapidjson.cc
  mcc/mock_gl.h mcc/mock_gl.cc
  mcc/mock_subscription.h
  mcc/test_color.cc
  mcc/test_file_resolver.cc
# IBOs
  mcc/ibo/test_ibo_factory.cc
  ${MCC_SHADER_TEST_SOURCES}
  ${MCC_PROGRAM_TEST_SOURCES}
  ${MCC_VAO_TEST_SOURCES}
  ${MCC_TEXTURE_TEST_SOURCES}
  ${MCC_IMAGE_TEST_SOURCES}
# Misc
  mcc/test_resolution.cc
  mcc/mock_glfw.h mcc/mock_glfw.cc)
target_link_libraries(${PROJECT_NAME}-tests
  ${PROJECT_NAME}
  GTest::gtest
  GTest::gmock)