#include <uv.h>
#include <benchmark/benchmark.h>

#include "mcc/flags.h"
#include "mcc/mesh/obj_parser.h"

namespace mcc {
  void BM_ObjParser_Parse(benchmark::State& state, const std::string& f) {
    const auto filename = FLAGS_resources + "/meshes" + f;
    const auto file = fopen(filename.c_str(), "rb");
    LOG_IF(FATAL, !file) << "failed to open " << filename;
    const auto filesize = GetFilesize(file);
    for(const auto& _ : state) {
      ObjParser parser(file);

      const auto start = uv_hrtime();
      LOG_IF(FATAL, !parser.ParseFile()) << "failed to obj file from: " << filename;
      const auto stop = uv_hrtime();
      state.SetIterationTime((stop - start) / NSEC_PER_SEC);
      state.SetLabel(f);
      const auto stats = parser.stats();
      state.counters["vertices"] = stats.vertices;
      state.counters["normals"] = stats.normals;
      state.counters["uvs"] = stats.uvs;
      state.counters["faces"] = stats.faces;
      state.SetBytesProcessed(filesize);
    }
  }

  BENCHMARK_CAPTURE(BM_ObjParser_Parse, sphere, "/sphere/sphere.obj")
    ->Name("ObjParser::Parse")
    ->Unit(benchmark::kMillisecond)
    ->Iterations(1);

  // BENCHMARK_CAPTURE(BM_ObjParser_Parse, cube, "/cube/cube.obj")
  //   ->Name("ObjParser::Parse")
  //   ->Iterations(1);
}