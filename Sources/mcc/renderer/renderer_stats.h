#ifndef MCC_RENDERER_STATS_H
#define MCC_RENDERER_STATS_H

#include <uv.h>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include "mcc/series.h"
#include "mcc/renderer/renderer_state.h"

namespace mcc::renderer {
  class RendererStats {
    friend class Renderer;
  protected:
    TimeSeries<10> time_;
    NumericSeries<uint64_t, 10> entities_;
    NumericSeries<uint64_t, 10> fps_;
    NumericSeries<uint64_t, 10> vertices_;

    inline void AppendTime(const uint64_t time) {
      time_.Append(time);
    }

    inline void AppendEntities(const uint64_t entities) {
      entities_.Append(entities);
    }

    inline void AppendVertices(const uint64_t vertices) {
      vertices_.Append(vertices);
    }
  public:
    RendererStats() = default;
    virtual ~RendererStats() = default;

    const TimeSeries<10>& time() const {
      return time_;
    }

    const NumericSeries<uint64_t, 10> entities() const {
      return entities_;
    }

    const NumericSeries<uint64_t, 10> vertices() const {
      return vertices_;
    }
  };
}

#endif //MCC_RENDERER_STATS_H