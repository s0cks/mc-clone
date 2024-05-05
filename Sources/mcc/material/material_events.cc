#include "mcc/material/material_events.h"
#include <sstream>
#include "mcc/material/material.h"

namespace mcc::material {
  const std::string& MaterialEvent::GetMaterialName() const {
    return GetMaterial()->GetName();
  }

  std::string MaterialCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "MaterialCreatedEvent(";
    ss << "material=" << GetMaterial()->ToString();
    ss << ")";
    return ss.str();
  }

  std::string MaterialDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "MaterialDestroyedEvent(";
    ss << "material=" << GetMaterial()->ToString();
    ss << ")";
    return ss.str();
  }
}