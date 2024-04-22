#include "mcc/ibo/index_buffer.h"

#include <sstream>

namespace mcc::ibo {
  static rx::subject<IboEvent*> events_;
  static std::set<Ibo*, Ibo::IdComparator> ibos_;

  rx::observable<IboEvent*> OnIboEvent() {
    return events_.get_observable();
  }

  rx::observable<Ibo*> GetRegisteredIbos() {
    return rx::observable<>::iterate(ibos_);
  }

  std::string Ibo::ToString() const {
    std::stringstream ss;
    ss << "Ibo(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}