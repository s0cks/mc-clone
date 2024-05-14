#ifndef MCC_SIPHASH_DIGEST_H
#define MCC_SIPHASH_DIGEST_H

#include <array>
#include "mcc/platform.h"

namespace mcc::hash {
  template<const uword KeySize>
  class SipHashDigest {
    typedef std::array<uint8_t, KeySize> Key;

    static inline uword
    RotateLeft(const uword val, const int32_t cnt) {
      return (val << cnt) | (val >> (64 - cnt));
    }

    static inline uword
    Read8(const Key& key, const uword o = 0) {
      return  static_cast<uword>(key[o + 0])
           | (static_cast<uword>(key[o + 1]) << 8)
           | (static_cast<uword>(key[o + 2]) << 16)
           | (static_cast<uword>(key[o + 3]) << 24)
           | (static_cast<uword>(key[o + 4]) << 32)
           | (static_cast<uword>(key[o + 5]) << 40)
           | (static_cast<uword>(key[o + 6]) << 48)
           | (static_cast<uword>(key[o + 7]) << 56);
    }
  protected:
    uword c_;
    uword d_;
    uword index_;
    uint8_t length_;
    uword v0_;
    uword v1_;
    uword v2_;
    uword v3_;
    uword m_;

    inline void Compress() {
      v0_ += v1_;
      v2_ += v3_;
      v1_ = RotateLeft(v1_, 13);
      v3_ = RotateLeft(v3_, 16);
      v1_ ^= v0_;
      v3_ ^= v2_;
      v0_ = RotateLeft(v0_, 32);
      v2_ += v1_;
      v0_ += v3_;
      v1_ = RotateLeft(v1_, 17);
      v3_ = RotateLeft(v3_, 21);
      v1_ ^= v2_;
      v3_ ^= v0_;
      v2_ = RotateLeft(v2_, 32);
    }

    inline void DigestBlock() {
      v3_ ^= m_;
      for(auto i = 0; i < c_; i++)
        Compress();
      v0_ ^= m_;
    }
  public:
    SipHashDigest(const Key& key,
                  const uword c,
                  const uword d):
      c_(0),
      d_(0),
      index_(0),
      length_(0),
      v0_(0),
      v1_(0),
      v2_(0),
      v3_(0),
      m_(0) {
      c_ = c;
      d_ = d;
      m_ = 0;
      const auto k0 = Read8(key);
      const auto k1 = Read8(key, 8);
      v0_ = (0x736f6d6570736575 ^ k0);
      v1_ = (0x646f72616e646f6d ^ k1);
      v2_ = (0x6c7967656e657261 ^ k0);
      v3_ = (0x7465646279746573 ^ k1);
      index_ = 0;
      length_ = 0;
    }
    virtual ~SipHashDigest() = default;

    void Update(const uint8_t byte) {
      ++length_;
      m_ |= (static_cast<uword>(byte & 0xff) << (index_++ * 8));
      if(index_ >= 8) {
        DigestBlock();
        index_ = 0;
        m_ = 0;
      }
    }

    void Update(const uint8_t* bytes, const uword num_bytes) {
      for(auto idx = 0; idx < num_bytes; idx++)
        Update(bytes[idx]);
    }

    void Update(const char* data, const uword length) {
      return Update((const uint8_t*) data, length);
    }

    void Update(const char* data) {
      return Update(data, strlen(data));
    }

    void Update(const std::string& data) {
      return Update(data.data(), data.length());
    }

    uword Digest() {
      while(index_ < 7)
        m_ |= 0 << (index_++ * 8);
      m_ |= (static_cast<uword>(length_)) << (index_ * 8);
      DigestBlock();
      v2_ ^= 0xff;
      for(auto i = 0; i < d_; i++)
        Compress();
      return v0_ ^ v1_ ^ v2_ ^ v3_;
    }
  };
}

#endif //MCC_SIPHASH_DIGEST_H