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
    ShaderType type_;
    ShaderCodeList code_;

    explicit ShaderUnit(const Metadata& meta,
                        const ShaderType type):
      Object(meta),
      type_(type),
      code_() {
    }
  public:
    ~ShaderUnit() override = default;
    std::string ToString() const override;

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
    void Append(const uri::Uri& code);
    void Append(const std::string& code);

    const ShaderCode*& operator[](const uword idx) {
      MCC_ASSERT(idx >= 0 && idx <= GetSize());
      return code_[idx];
    }

    const ShaderCode* operator[](const uword idx) const {
      MCC_ASSERT(idx >= 0 && idx <= GetSize());
      return code_[idx];
    }
  public:
    static inline ShaderUnit*
    New(const Metadata& meta, const ShaderType type) {
      return new ShaderUnit(meta, type);
    }

    static inline ShaderUnit*
    New(const ShaderType type) {
      Metadata meta;
      return New(meta, type);
    }

#define DEFINE_NEW_SHADER_UNIT_BY_TYPE(Name, Ext, GlValue)          \
    static inline ShaderUnit*                                       \
    New##Name##ShaderUnit(const Metadata& meta = {}) {              \
      return New(meta, k##Name##Shader);                            \
    }

    FOR_EACH_SHADER_TYPE(DEFINE_NEW_SHADER_UNIT_BY_TYPE)
#undef DEFINE_NEW_SHADER_UNIT_BY_TYPE
  };
}

#endif //MCC_SHADER_UNIT_H