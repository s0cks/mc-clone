find_package(benchmark REQUIRED)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${CMAKE_PROJECT_NAME}-benchmarks
  main.cc
  mcc/bm_murmur.h mcc/bm_murmur.cc
  mcc/bm_component.h mcc/bm_component.cc
  mcc/mesh/bm_obj_parser.cc)
target_link_libraries(${CMAKE_PROJECT_NAME}-benchmarks
  ${CMAKE_PROJECT_NAME}
  benchmark::benchmark
  benchmark::benchmark_main)