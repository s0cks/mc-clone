#ifndef MCC_SERIES_H
#define MCC_SERIES_H

#include <functional>
#include <algorithm>

#include "mcc/rx.h"
#include "mcc/circular_buffer.h"

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

    rx::observable<T> ToObservable() const {
      return rx::observable<>::create<T>([this](rx::subscriber<T> s) {
        for(const auto& value : data_) {
          s.on_next(value);
        }
        s.on_completed();
      });
    }

    explicit operator rx::observable<T> () const {
      return ToObservable();
    }
  };

  template<typename T, const uint64_t Capacity = 10>
  class NumericSeries : public Series<uint64_t, Capacity> {
  public:
    NumericSeries() = default;
    ~NumericSeries() override = default;

    inline uint64_t average() const {
      return Series<uint64_t, Capacity>::ToObservable()
        .as_blocking()
        .average();
    }

    inline uint64_t max() const {
      return Series<uint64_t, Capacity>::ToObservable()
        .as_blocking()
        .max();
    }

    inline uint64_t min() const {
      return Series<uint64_t, Capacity>::ToObservable()
        .as_blocking()
        .min();
    }

    explicit operator rx::observable<uint64_t> () const {
      return Series<uint64_t, Capacity>::ToObservable();
    }
  };

  template<const uint64_t Capacity = 10>
  class TimeSeries : public NumericSeries<uint64_t, Capacity> {
  public:
    TimeSeries() = default;
    ~TimeSeries() override = default;

    explicit operator rx::observable<uint64_t> () const {
      return Series<uint64_t, Capacity>::ToObservable();
    }
  };
}

#endif //MCC_SERIES_H