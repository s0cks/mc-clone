#ifndef MCC_BENCHMARK_MURMUR_H
#define MCC_BENCHMARK_MURMUR_H

#include <benchmark/benchmark.h>
#include "mcc/murmur.h"

namespace mcc {
  void BM_Murmur(benchmark::State& state);
}

#endif //MCC_BENCHMARK_MURMUR_H