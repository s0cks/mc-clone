find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB MCC_SHADER_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/shader/*.cc")

file(GLOB MCC_URI_TEST_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri_*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/mcc/*_uri_*.cc")

add_executable(${PROJECT_NAME}-tests
  main.cc
  ${MCC_URI_TEST_SOURCES}
  mcc/mock_stdio.h mcc/mock_stdio.cc
  mcc/test_cache.h mcc/test_cache.cc
  mcc/test_rapidjson.h mcc/test_rapidjson.cc
  mcc/mock_gl.h mcc/mock_gl.cc
  mcc/mock_subscription.h
  mcc/test_color.cc
  mcc/test_file_resolver.cc
# IBOs
  mcc/ibo/test_ibo_factory.cc
  ${MCC_SHADER_TEST_SOURCES}
# Textures
  mcc/texture/mock_texture.h
  mcc/texture/test_texture_filter.cc
  mcc/texture/test_texture_alignment.cc
# Misc
  mcc/test_resolution.cc
  mcc/mock_glfw.h mcc/mock_glfw.cc)
target_link_libraries(${PROJECT_NAME}-tests
  ${PROJECT_NAME}
  GTest::gtest
  GTest::gmock)