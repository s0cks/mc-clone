#include "mcc/bm_murmur.h"

namespace mcc {
  void BM_Murmur(benchmark::State& state) {
    static constexpr const char* kExampleString = "This is an example string.";
    for(const auto& _ : state)
      Murmur(kExampleString, strlen(kExampleString));
  }

  BENCHMARK(BM_Murmur);
}