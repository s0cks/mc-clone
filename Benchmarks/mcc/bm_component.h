#ifndef MCC_BENCHMARK_COMPONENT_H
#define MCC_BENCHMARK_COMPONENT_H

#include <benchmark/benchmark.h>

#include "mcc/component/component.h"

namespace mcc::component {
  void BM_ComponentRegister(benchmark::State& state);
  void BM_ComponentTableRegister(benchmark::State& state);
  void BM_GetComponentId(benchmark::State& state);
}

#endif //MCC_BENCHMARK_COMPONENT_H