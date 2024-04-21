#ifndef MCC_COLOR_H
#define MCC_COLOR_H

#include "mcc/glm.h"

namespace mcc {
  namespace rgb {
    typedef glm::u8vec4 Color;

    static inline constexpr const Color
    Hex(const uint32_t value) {
      return {
        static_cast<uint8_t>((value >> 16) & 0xFF),
        static_cast<uint8_t>((value >> 8) & 0xFF),
        static_cast<uint8_t>(value & 0xFF),
        0xFF,
      };
    }

    static constexpr const Color kBlack     = Hex(0x000000);
    static constexpr const Color kWhite     = Hex(0xFFFFFF);
    static constexpr const Color kRed       = Hex(0xFF0000);
    static constexpr const Color kGreen     = Hex(0x00FF00);
    static constexpr const Color kBlue      = Hex(0x0000FF);
  }
  using namespace rgb;
}

#endif //MCC_COLOR_H