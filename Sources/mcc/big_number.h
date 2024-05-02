#ifndef MCC_BIG_NUMBER_H
#define MCC_BIG_NUMBER_H

#include <string>
#include <cstdint>

namespace mcc {
  class BigNumber {
  protected:
    BigNumber() = default;
  public:
    virtual ~BigNumber() = default;
    virtual const uint8_t* data() const = 0;
    virtual uint64_t size() const = 0;
    virtual void clear() = 0;

    const uint8_t* const_begin() const {
      return data();
    }

    const uint8_t* const_end() const {
      return data() + size();
    }
  };

  template<const uint64_t Size>
  class BigNumberTemplate : public BigNumber { //TODO: convert to words?
  protected:
    uint8_t data_[Size];

    constexpr BigNumberTemplate():
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
    }
    constexpr BigNumberTemplate(const uint8_t* bytes, const uint64_t num_bytes):
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
      memcpy(&data_[0], &bytes[0], std::min(Size, num_bytes));
    }
    constexpr BigNumberTemplate(const BigNumberTemplate<Size>& rhs):
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
      memcpy(&data_[0], &rhs.data_[0], Size);
    }
  public:
    ~BigNumberTemplate() override = default;

    const uint8_t* data() const override {
      return data_;
    }

    uint64_t size() const override {
      return Size;
    }

    void clear() override {
      memset(&data_[0], 0, sizeof(data_));
    }
  };
}

#endif //MCC_BIG_NUMBER_H