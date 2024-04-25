#include "mcc/mesh/mesh_events.h"
#include <sstream>
#include "mcc/mesh/mesh.h"

namespace mcc::mesh {
  std::string MeshCreatedEvent::ToString() const {
    std::stringstream ss;
    ss << "MeshCreatedEvent(";
    ss << "mesh=" << GetMesh()->ToString();
    ss << ")";
    return ss.str();
  }
  
  std::string MeshDestroyedEvent::ToString() const {
    std::stringstream ss;
    ss << "MeshDestroyedEvent(";
    ss << "mesh=" << GetMesh()->ToString();
    ss << ")";
    return ss.str();
  }
}