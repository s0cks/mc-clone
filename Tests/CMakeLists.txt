find_package(GTest REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME}-tests
  main.cc
  mcc/mesh/test_vertex.h mcc/mesh/test_vertex.cc
  mcc/event/test_event_bus.h mcc/event/test_event_bus.cc)
target_link_libraries(${PROJECT_NAME}-tests
  ${PROJECT_NAME}
  Threads::Threads
  GTest::gtest
  GTest::gmock
  glog::glog
  ${GFLAGS_LIBRARIES})