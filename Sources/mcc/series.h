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

    rxcpp::observable<T> asObservable() const {
      return rxcpp::observable<>::iterate(this);
    }
  };

  template<typename N, const uint64_t Capacity>
  class NumericSeries : public Series<N, Capacity> {
  public:
    struct Stats {
      N average;
      N sum;
      N min;
      N max;
    };
  protected:
    NumericSeries() = default;
  public:
    ~NumericSeries() override = default;
  };
}

#endif //MCC_SERIES_H