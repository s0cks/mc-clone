#ifndef MCC_MURMUR_H
#define MCC_MURMUR_H

#include <cstdlib>
#include <cstdint>
#include <string>

namespace mcc {
  static inline uint32_t
  Murmur(const char* data, const uint64_t len) {
    uint32_t c1 = 0xCC9E2D51;
    uint32_t c2 = 0x1B873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xE6546B64;
    uint32_t h = 0xA;
    uint32_t k = 0;
    uint8_t* d = (uint8_t*) data;
    int l = (int) (len / 4);
    uint32_t* chunks = (uint32_t*) (d + l * 4);
    uint8_t* tail = d + l * 4;
    for(int i = -l; i != 0; i++){
        k = chunks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }
    k = 0;
    switch(len & 3){
        case 3: k ^= (tail[2] << 16);
        case 2: k ^= (tail[1] << 8);
        case 1:{
            k ^= tail[0];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;
            h ^= k;
        }
        default:{
            break;
        }
    }

    h ^= len;
    h ^= (h >> 16);
    h *= 0x85EBCA6B;
    h ^= (h >> 13);
    h *= 0xC2B2AE35;
    h ^= (h >> 16);
    return h;
  }

  static inline uint32_t
  Murmur(const std::string& value) {
    return Murmur(value.data(), value.length());
  }
}

#endif //MCC_MURMUR_H