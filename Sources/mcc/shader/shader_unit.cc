#include "mcc/shader/shader_unit.h"
#include <sstream>
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  uint256 ShaderUnit::GetHash() const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement, H() => MerkleTree(|code_|)
    return {};
  }

  void ShaderUnit::Append(const ShaderCode* code) {
    MCC_ASSERT(code);
    MCC_ASSERT(GetType() == code->GetType());
    code_.push_back(code);
  }

  std::string ShaderUnit::ToString() const {
    std::stringstream ss;
    ss << "ShaderUnit(";
    ss << "name=" << GetName() << ", ";
    ss << "type=" << GetType() << ", ";
    if(!IsEmpty())
      ss << "code=" << GetHash();
    ss << ")";
    return ss.str();
  }
}