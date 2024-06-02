#include "mcc/shader/shader_unit.h"
#include <sstream>
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  uint256 ShaderUnit::GetHash() const {
    NOT_IMPLEMENTED(ERROR); //TODO: implement, H() => MerkleTree(|code_|)
    return {};
  }

  std::string ShaderUnit::ToString() const {
    std::stringstream ss;
    ss << "ShaderUnit(";
    ss << "meta=" << GetMeta() << ", ";
    ss << "type=" << GetType() << ", ";
    if(!IsEmpty())
      ss << "code=" << GetHash();
    ss << ")";
    return ss.str();
  }
}