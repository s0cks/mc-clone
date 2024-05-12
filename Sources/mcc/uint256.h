#ifndef MCC_UINT256_H
#define MCC_UINT256_H

#include "mcc/common.h"
#include "mcc/big_number.h"

namespace mcc {
  static constexpr const uint64_t kUInt256Size = 256 / 8;
  class uint256 : public BigNumberTemplate<kUInt256Size> {
    DEFINE_DEFAULT_COPYABLE_TYPE(uint256);
  public:
    static inline int
    Compare(const uint256& lhs, const uint256& rhs) {
      return memcmp(lhs.data(), rhs.data(), kUInt256Size);
    }
  public:
    uint256() = default;
    uint256(const uint8_t* bytes, const uword num_bytes):
      BigNumberTemplate<kUInt256Size>(bytes, num_bytes) {
    }
    uint256(const uint32_t a,
            const uint32_t b,
            const uint32_t c,
            const uint32_t d,
            const uint32_t e,
            const uint32_t f,
            const uint32_t g,
            const uint32_t h):
      uint256() {
      uint32_t data[] = { a, b, c, d, e, f, g, h };
      CopyFrom((const uint8_t*) data, kSizeInBytes);
    }
#if defined(ARCH_IS_X64) || defined(ARCH_IS_ARM64)
    uint256(const uint64_t a,
            const uint64_t b,
            const uint64_t c,
            const uint64_t d):
      uint256() {
      const uint64_t data[] = { a, b, c, d };
      CopyFrom((const uint8_t*) data, kSizeInBytes);
    }
#endif //ARCH_IS_X64 || ARCH_IS_ARM64
    ~uint256() override = default;

    uword& operator[](const uword idx) {
      MCC_ASSERT(idx >= 0 && idx <= size());
      return data_[idx];
    }

    uword operator[](const uword idx) const {
      MCC_ASSERT(idx >= 0 && idx <= size());
      return data_[idx];
    }

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

    explicit operator std::string() const {
      return ToHexString();
    }

    friend std::ostream& operator<<(std::ostream& stream, const uint256& rhs) {
      return stream << ((const std::string&) rhs);
    }
  };
}

#endif //MCC_UINT256_H