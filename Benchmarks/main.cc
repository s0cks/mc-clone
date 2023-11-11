#include <glog/logging.h>
#include <gflags/gflags.h>
#include <benchmark/benchmark.h>

int main(int argc, char** argv) {
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();
  ::google::ParseCommandLineFlags(&argc, &argv, false);
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return EXIT_SUCCESS;
}