#ifndef MCC_SERIES_H
#define MCC_SERIES_H

#include <functional>
#include <algorithm>
#include "mcc/circular_buffer.h"

#include <rxcpp/rx.hpp>
#include <rxcpp/rx-includes.hpp>

namespace mcc {
  template<typename T, const uint64_t Capacity>
  class Series {
  protected:
    CircularBuffer<T, Capacity> data_;

    Series():
      data_() {
    }
  public:
    virtual ~Series() = default;

    void Append(const T value) {
      data_.put(value);
    }

    T* begin() {
      return data_.begin();
    }

    T* begin() const {
      return data_.begin();
    }

    T* end() {
      return data_.end();
    }

    T* end() const {
      return data_.end();
    }

    explicit operator rx::observable<T> () const {
      return rx::observable<>::create<T>([this](rx::subscriber<T> s) {
        for(const auto& value : data_) {
          s.on_next(value);
        }
        s.on_completed();
      });
    }
  };

  template<const uint64_t Capacity = 10>
  class TimeSeries : public Series<uint64_t, Capacity> {
  public:
    TimeSeries() = default;
    ~TimeSeries() override = default;
  };
}

#endif //MCC_SERIES_H