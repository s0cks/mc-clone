find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}-tests
  main.cc
  mcc/test_uri.cc
  mcc/test_cache.h mcc/test_cache.cc
  mcc/test_rapidjson.h mcc/test_rapidjson.cc
  mcc/mesh/test_vertex.h mcc/mesh/test_vertex.cc
  mcc/mesh/test_mesh.h mcc/mesh/test_mesh.cc
  mcc/mock_gl.h mcc/mock_gl.cc
  mcc/mock_subscription.h
# IBOs
  mcc/ibo/test_ibo_factory.cc
# Shaders
  mcc/shader/test_program_linker.cc
  mcc/shader/test_shader_compiler.cc
  mcc/shader/test_shader_spec_json.cc
# Textures
  mcc/texture/mock_texture.h
  mcc/texture/test_texture_filter.cc
  mcc/texture/test_texture_alignment.cc
# Materials
  mcc/material/test_material.cc
  mcc/material/test_material_json.cc
  mcc/material/test_material_loader_default.cc
# Misc
  mcc/mock_glfw.h mcc/mock_glfw.cc
  mcc/mesh/test_obj_parser.h mcc/mesh/test_obj_parser.cc)
target_link_libraries(${PROJECT_NAME}-tests
  ${PROJECT_NAME}
  Threads::Threads
  GTest::gtest
  GTest::gmock
  glog::glog
  ${LIBUV_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${LEVELDB_LIBRARIES})