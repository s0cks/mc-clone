#include <string>

#include <uv.h>
#include <benchmark/export.h>
#include <benchmark/benchmark.h>

#include "mcc/bm_component.h"
#include "mcc/physics/transform.h"
#include "mcc/component/component_state.h"

namespace mcc::component {
  static void upper_cost_of_pausing(benchmark::State& state) {
      int32_t a = std::rand(), c = 0;
      for (auto _ : state) {
          state.PauseTiming();
          ++a;
          state.ResumeTiming();
          ::benchmark::DoNotOptimize(c += a);
      }
  }

  double nround (double n, unsigned int c) {
    double marge = pow (10, c);
    double up    = n * marge;
    double ret   = round (up) / marge;
    return ret; 
  }

  static constexpr const auto kNameSize = 14;
  void BM_ComponentRegister(benchmark::State& state) {
    for(const auto& _ : state) {
      const auto num_components = state.range(0);
      uint64_t total = 0;
      for(uint64_t idx = 0; idx < num_components; idx++) {
        char name[kNameSize];
        snprintf(name, kNameSize, "Component%04llu", idx++);

        const auto start = uv_hrtime();
        Component::Register(name);
        const auto stop = uv_hrtime();

        total += (stop - start);
      }
      state.SetIterationTime((total * 1.0) / NSEC_PER_SEC);
      Component::ClearRegisteredComponents();
    }
  }

  void BM_ComponentGet(benchmark::State& state) {
    uint64_t idx = 0;
    for(const auto& _ : state) {
      char name[kNameSize];
      snprintf(name, kNameSize, "Component%04llu", idx++);

      const auto start = std::chrono::high_resolution_clock::now();
      Component::GetComponentId(name);
      const auto stop = std::chrono::high_resolution_clock::now();
      state.SetIterationTime(std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count());
    }
  }

  BENCHMARK(BM_ComponentRegister)
    ->DenseRange(64, 512, 64)
    ->Name("Component::Register")
    ->UseManualTime();
  BENCHMARK(upper_cost_of_pausing);
}