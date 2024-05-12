#ifndef MCC_BIG_NUMBER_H
#define MCC_BIG_NUMBER_H

#include <string>
#include <cstdint>
#include "mcc/platform.h"

namespace mcc {
  class BigNumber {
  protected:
    BigNumber() = default;
  public:
    virtual ~BigNumber() = default;
    virtual const uword* data() const = 0;
    virtual uword size() const = 0;
    virtual void clear() = 0;

    const uword* const_begin() const {
      return data();
    }

    const uword* const_end() const {
      return data() + size();
    }
  };

  template<const uword NumberOfBits>
  class BigNumberTemplate : public BigNumber {
  public:
    static constexpr const auto kSizeInBits = NumberOfBits;
    static constexpr const auto kSizeInBytes = kSizeInBits / kBitsPerByte;
    static constexpr const auto kSizeInWords = kSizeInBits / kBitsPerWord;
  protected:
    uword data_[kSizeInWords];

    constexpr BigNumberTemplate():
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
    }
    constexpr BigNumberTemplate(const uint8_t* bytes, const uword num_bytes):
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
      memcpy(&data_[0], &bytes[0], std::min(kSizeInBytes, num_bytes));
    }
    constexpr BigNumberTemplate(const BigNumberTemplate<NumberOfBits>& rhs):
      BigNumber(),
      data_() {
      memset(&data_[0], 0, sizeof(data_));
      memcpy(&data_[0], &rhs.data_[0], kSizeInBytes);
    }

    inline void
    CopyFrom(const uint8_t* bytes, const uword num_bytes) {
      const auto total_bytes = std::min(num_bytes, kSizeInBytes);
      memcpy(&data_[0], &bytes[0], total_bytes);
    }
  public:
    ~BigNumberTemplate() override = default;

    const uword* data() const override {
      return data_;
    }

    uword size() const override {
      return kSizeInWords;
    }

    void clear() override {
      memset(&data_[0], 0, sizeof(data_));
    }

    virtual std::string ToHexString() const {
      static const char *hexDigits = "0123456789ABCDEF";
      std::string hexString;
      hexString.reserve(size() * 2);
      std::for_each(
          const_begin(),
          const_end(),
          [&hexString](const uword word) {
            hexString.push_back(hexDigits[word >> 16]);
            hexString.push_back(hexDigits[word >> 8]);
            hexString.push_back(hexDigits[word >> 4]);
            hexString.push_back(hexDigits[word & 0x0F]);
          });
      return hexString;
    }
    
    virtual std::string ToBinaryString() const {
      std::stringstream ss;
      std::bitset<kSizeInBits> bits;
      for(auto idx = 0; idx < size(); idx++){
        auto curr = data()[idx];
        int offset = idx * CHAR_BIT;
        for(int bit = 0; bit < CHAR_BIT; bit++){
          bits[offset] = curr & 1;
          offset++;
          curr >>= 1;
        }
      }
      ss << bits;
      return ss.str();
    }
  };
}

#endif //MCC_BIG_NUMBER_H