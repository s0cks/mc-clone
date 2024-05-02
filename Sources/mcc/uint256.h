#ifndef MCC_UINT256_H
#define MCC_UINT256_H

#include "mcc/common.h"
#include "mcc/big_number.h"

namespace mcc {
  static constexpr const uint64_t kUInt256Size = 256 / 8;
  class uint256 : public BigNumberTemplate<kUInt256Size> {
  public:
    static inline int
    Compare(const uint256& lhs, const uint256& rhs) {
      return memcmp(lhs.data(), rhs.data(), kUInt256Size);
    }
  public:
    uint256() = default;
    uint256(const uint8_t* bytes, const uint64_t num_bytes):
      BigNumberTemplate<kUInt256Size>(bytes, num_bytes) {
    }
    uint256(const uint256& rhs) = default;
    ~uint256() override = default;

    std::string ToHexString() const;
    std::string ToBinaryString() const;

    uint8_t& operator[](const size_t idx) {
      MCC_ASSERT(idx >= 0 && idx <= kUInt256Size);
      return data_[idx];
    }

    uint256& operator=(const uint256& rhs) = default;

    bool operator==(const uint256& rhs) const {
      return Compare(*this, rhs) == 0;
    }

    bool operator!=(const uint256& rhs) const {
      return Compare(*this, rhs) != 0;
    }

    bool operator<(const uint256& rhs) const {
      return Compare(*this, rhs) < 0;
    }

    bool operator>(const uint256& rhs) const {
      return Compare(*this, rhs) > 0;
    }
  };
}

#endif //MCC_UINT256_H