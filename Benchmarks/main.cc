#include <glog/logging.h>
#include <gflags/gflags.h>
#include <benchmark/benchmark.h>

int main(int argc, char** argv) {
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::benchmark::Initialize(&argc, argv);
  ::google::ParseCommandLineFlags(&argc, &argv, false);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return EXIT_SUCCESS;
}