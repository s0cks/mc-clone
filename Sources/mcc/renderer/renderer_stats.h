#ifndef MCC_RENDERER_STATS_H
#define MCC_RENDERER_STATS_H

#include <uv.h>
#include <cmath>
#include <cstdint>
#include <algorithm>

#include "mcc/series.h"
#include "mcc/renderer/renderer_state.h"

namespace mcc::renderer {
  struct RendererSample {
    uint64_t duration;
    uint64_t entities;

    friend std::ostream& operator<<(std::ostream& stream, const RendererSample& rhs) {
      stream << "RendererSample(";
      stream << "duration=" << rhs.duration << ", ";
      stream << "entities=" << rhs.entities;
      stream << ")";
      return stream;
    }
  public:
    struct DurationComparator {
    public:
      bool operator() (const RendererSample& lhs, const RendererSample& rhs) {
        return lhs.duration < rhs.duration;
      }
    };

    struct EntitiesComparator {
    public:
      bool operator()(const RendererSample& lhs, const RendererSample& rhs) {
        return lhs.entities < rhs.entities;
      }
    };
  };

  static constexpr const uint64_t kNumberOfRendererSamples = 60;
  class RendererSampleSeries : public Series<RendererSample, kNumberOfRendererSamples> {  public:
    RendererSampleSeries():
      Series() {
    }
    ~RendererSampleSeries() = default;

    uint64_t GetTotalNanoseconds() const {
      uint64_t total = 0;
      for(auto& sample : data_)
        total += sample.duration;
      return total;
    }

    uint64_t GetAvgDuration() const {
      return GetTotalNanoseconds() / data_.size();
    }

    uint64_t GetMaxDuration() const {
      return std::max_element(data_.begin(), data_.end(), RendererSample::DurationComparator{})->duration;
    }

    uint64_t GetMinDuration() const {
      return std::min_element(data_.begin(), data_.end(), RendererSample::DurationComparator{})->duration;
    }

    uint64_t GetTotalEntities() const {
      uint64_t total = 0;
      for(auto& sample : data_) {
        total += sample.entities;
      }
      return total;
    }

    uint64_t GetAvgEntities() const {
      return GetTotalEntities() / data_.size();
    }

    uint64_t GetMaxEntities() const {
      return std::max_element(data_.begin(), data_.end(), RendererSample::EntitiesComparator{})->entities;
    }

    uint64_t GetMinEntities() const {
      return std::min_element(data_.begin(), data_.end(), RendererSample::EntitiesComparator{})->entities;
    }

    void operator<<(const RendererSample& rhs) {
      data_.put(rhs);
    }
  };
}

#endif //MCC_RENDERER_STATS_H