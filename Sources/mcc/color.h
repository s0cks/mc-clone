#ifndef MCC_COLOR_H
#define MCC_COLOR_H

#include <ostream>
#include "mcc/glm.h"

namespace mcc {
  namespace rgb {
    typedef glm::u8vec4 Color;

    static inline constexpr uint8_t
    GetAlphaComponent(const uint32_t& rhs) {
      return (rhs >> 24) & 0xFF;
    }

    static inline constexpr uint8_t
    GetRedComponent(const uint32_t& rhs) {
      return (rhs >> 16) & 0xFF;
    }

    static inline constexpr uint8_t
    GetGreenComponent(const uint32_t& rhs) {
      return (rhs >> 8) & 0xFF;
    }

    static inline constexpr uint8_t
    GetBlueComponent(const uint32_t& rhs) {
      return rhs & 0xFF;
    }

    static inline constexpr const Color
    Hex(const uint32_t value) {
      return {
        GetRedComponent(value),
        GetGreenComponent(value),
        GetBlueComponent(value),
        0xFF
      };
    }

    enum ColorComponent : int8_t {
      kRedComponent = 0,
      kGreenComponent = 1,
      kBlueComponent = 2,
      kAlphaComponent = 3,
    };

    static inline std::ostream&
    operator<<(std::ostream& stream, const ColorComponent& rhs) {
      switch(rhs) {
        case kRedComponent: return stream << "Red (" << static_cast<int>(rhs) << ")";
        case kGreenComponent: return stream << "Green (" << static_cast<int>(rhs) << ")";
        case kBlueComponent: return stream << "Blue (" << static_cast<int>(rhs) << ")";
        case kAlphaComponent: return stream << "Alpha (" << static_cast<int>(rhs) << ")";
      }
    }

    static constexpr const Color kBlack     = Hex(0x000000);
    static constexpr const Color kWhite     = Hex(0xFFFFFF);
    static constexpr const Color kRed       = Hex(0xFF0000);
    static constexpr const Color kGreen     = Hex(0x00FF00);
    static constexpr const Color kBlue      = Hex(0x0000FF);

    static inline constexpr Color
    operator&(const Color& lhs, const Color& rhs) {
      return Color {
        lhs[kRedComponent] & rhs[kRedComponent],
        lhs[kGreenComponent] & rhs[kGreenComponent],
        lhs[kBlueComponent] & rhs[kBlueComponent],
        lhs[kAlphaComponent] & rhs[kAlphaComponent],
      };
    }

    static inline constexpr Color
    operator&(const Color& lhs, const uint32_t& rhs) {
      return Color {
        lhs[kRedComponent] & GetRedComponent(rhs),
        lhs[kGreenComponent] & GetGreenComponent(rhs),
        lhs[kBlueComponent] & GetBlueComponent(rhs),
        lhs[kAlphaComponent],
      };
    }

    static inline std::ostream&
    operator<<(std::ostream& stream, const Color& rhs) {
      stream << "rgb::Color(";
      stream << "r=" << rhs.r << ", ";
      stream << "g=" << rhs.g << ", ";
      stream << "b=" << rhs.b << ", ";
      stream << "a=" << rhs.a;
      stream << ")";
      return stream;
    }
  }
  using namespace rgb;
}

#endif //MCC_COLOR_H