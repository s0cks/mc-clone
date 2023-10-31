#ifndef MCC_SERIES_H
#define MCC_SERIES_H

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
  };
}

#endif //MCC_SERIES_H