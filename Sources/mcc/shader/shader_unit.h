#ifndef MCC_SHADER_UNIT_H
#define MCC_SHADER_UNIT_H

#include <vector>
#include "mcc/common.h"
#include "mcc/object.h"
#include "mcc/uint256.h"
#include "mcc/shader/shader_type.h"

namespace mcc::shader {
  class ShaderCode;
  class ShaderUnit : public Object {
    typedef std::vector<const ShaderCode*> ShaderCodeList;
  protected:
    std::string name_;
    ShaderType type_;
    ShaderCodeList code_;
  public:
    explicit ShaderUnit(const std::string& name,
                        const ShaderType type):
      Object(),
      name_(name),
      type_(type),
      code_() {
    }
    ~ShaderUnit() override = default;
    std::string ToString() const override;

    const std::string& GetName() const {
      return name_;
    }

    ShaderType GetType() const {
      return type_;
    }

    uword GetSize() const {
      return code_.size();
    }

    bool IsEmpty() const {
      return code_.empty();
    }

    ShaderCodeList::const_iterator begin() const {
      return std::begin(code_);
    }

    ShaderCodeList::const_iterator end() const {
      return std::end(code_);
    }

    const ShaderCodeList& GetCode() const {
      return code_;
    }

    uint256 GetHash() const;
    void Append(const ShaderCode* code);

    const ShaderCode*& operator[](const uword idx) {
      MCC_ASSERT(idx >= 0 && idx <= GetSize());
      return code_[idx];
    }

    const ShaderCode* operator[](const uword idx) const {
      MCC_ASSERT(idx >= 0 && idx <= GetSize());
      return code_[idx];
    }
  };
}

#endif //MCC_SHADER_UNIT_H