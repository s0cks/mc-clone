find_package(benchmark REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}-benchmarks
  mcc/test_benchmark0.cc)
target_link_libraries(${CMAKE_PROJECT_NAME}-benchmarks
  ${CMAKE_PROJECT_NAME}
  benchmark::benchmark
  benchmark::benchmark_main)