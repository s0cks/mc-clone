#include <benchmark/benchmark.h>

static inline void
BM_StringCreation(benchmark::State& state) {
  for(auto _ : state)
    std::string empty;
}

BENCHMARK(BM_StringCreation);

BENCHMARK_MAIN();