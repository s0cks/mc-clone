#include "mcc/shader/shader_unit.h"
#include <sstream>
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  uint256 ShaderUnit::GetHash() const {
    NOT_IMPLEMENTED(FATAL); //TODO: implement, H() => MerkleTree(|code_|)
    return {};
  }

  void ShaderUnit::Append(const uri::Uri& code) {
    MCC_ASSERT(code.HasScheme("file", "shader"));
    MCC_ASSERT(code.HasExtension());
    return Append(ShaderCode::FromFile(code));
  }

  void ShaderUnit::Append(const std::string& code) {
    if(StartsWith(code, "file:")) {
      return Append(uri::Uri(code));
    } else if(StartsWith(code, "shader:")) {
      return Append(uri::Uri(code));
    }
    NOT_IMPLEMENTED(FATAL); //TODO: implement
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