find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}-tests
  main.cc
  mcc/test_rapidjson.h mcc/test_rapidjson.cc
  mcc/mesh/test_vertex.h mcc/mesh/test_vertex.cc
  mcc/component/test_component_state_table.h mcc/component/test_component_state_table.cc)
target_link_libraries(${PROJECT_NAME}-tests
  ${PROJECT_NAME}
  Threads::Threads
  GTest::gtest
  GTest::gmock
  glog::glog
  ${GFLAGS_LIBRARIES})