#include "mcc/shader/shader_unit.h"
#include <sstream>
#include "mcc/merkle.h"
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  std::string ShaderUnit::ToString() const {
    std::stringstream ss;
    ss << "ShaderUnit(";
    ss << "meta=" << GetMeta() << ", ";
    ss << "type=" << GetType() << ", ";
    if(!IsEmpty())
      ss << "hash=" << GetHash();
    ss << ")";
    return ss.str();
  }
}