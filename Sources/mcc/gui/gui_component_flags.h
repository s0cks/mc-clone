#ifndef MCC_GUI_COMPONENT_FLAGS_H
#define MCC_GUI_COMPONENT_FLAGS_H

#include <ostream>
#include "mcc/platform.h"
#include "mcc/bitfield.h"
#include "mcc/relaxed_atomic.h"

namespace mcc::gui {
  typedef uword RawComponentFlags;

  static constexpr const RawComponentFlags kNoComponentFlags = 0x0;

  class ComponentFlags {
  public:
    enum Layout {
      kResizableOffset = 0,
      kEnteredOffset = 1,
    };
  private:
    template<typename T, const uword Position, const uword Size>
    class ComponentFlagsField : public BitField<RawComponentFlags, T, Position, Size> {};

    template<const uword Position>
    class ComponentFlagsFlag : public ComponentFlagsField<bool, Position, 1>{};
  public:
    class ResizableFlag : public ComponentFlagsFlag<kResizableOffset>{};
    class EnteredFlag : public ComponentFlagsFlag<kEnteredOffset>{};
  protected:
    RelaxedAtomic<RawComponentFlags> raw_;
  public:
    constexpr ComponentFlags(const RawComponentFlags raw = kNoComponentFlags):
      raw_(raw) {
    }
    ~ComponentFlags() = default;
    
    void Set(const RawComponentFlags raw) {
      raw_ = raw;
    }

    inline void Clear() {
      return Set(kNoComponentFlags);
    }

    bool IsResizable() const {
      return ResizableFlag::Decode(raw());
    }

    void SetResizable(const bool value = true) {
      return Set(ResizableFlag::Update(value, raw()));
    }

    inline void ClearResizable() {
      return SetResizable(false);
    }

    bool IsEntered() const {
      return EnteredFlag::Decode(raw());
    }

    void SetEntered(const bool value = true) {
      return Set(EnteredFlag::Update(value, raw()));
    }

    inline void ClearEntered() {
      return SetEntered(false);
    }

    RawComponentFlags raw() const {
      return (RawComponentFlags) raw_;
    }

    operator RawComponentFlags() const {
      return raw();
    }

    bool operator==(const ComponentFlags& rhs) const {
      return raw() == rhs.raw();
    }

    bool operator!=(const ComponentFlags& rhs) const {
      return raw() != rhs.raw();
    }

    friend std::ostream& operator<<(std::ostream& stream, const ComponentFlags& rhs) {
      stream << "ComponentFlags(";
      stream << "resizable=" << (rhs.IsResizable() ? 'y' : 'n') << ", ";
      stream << "entered=" << (rhs.IsEntered() ? 'y' : 'n');
      stream << ")";
      return stream;
    }
  public:
    static inline constexpr ComponentFlags 
    None() {
      return kNoComponentFlags;
    }

    static inline constexpr ComponentFlags
    Resizable(const bool value = true) {
      return None() | ResizableFlag::Encode(value);
    }
  };
}

#endif //MCC_GUI_COMPONENT_FLAGS_H