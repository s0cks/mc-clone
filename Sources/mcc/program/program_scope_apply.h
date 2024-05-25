#ifndef MCC_PROGRAM_SCOPE_H
#error "Please #include <mcc/program/program_scope.h> instead."
#endif //MCC_PROGRAM_SCOPE_H

#ifndef MCC_PROGRAM_SCOPE_APPLY_H
#define MCC_PROGRAM_SCOPE_APPLY_H

#include "mcc/ubo/ubo_id.h"
#include "mcc/texture/texture_id.h"
#include "mcc/program/program_scope.h"

namespace mcc::program {
  typedef GLint UniformLocation;
  typedef GLint UniformBlockIndex;

  class ApplyProgramScope : public ProgramScope {
  public:
    explicit ApplyProgramScope(Program* program);
    ~ApplyProgramScope() override;
    virtual UniformLocation GetUniformLocation(const char* name) const; //TODO: cache value, reduce visibility?
    virtual UniformBlockIndex GetUniformBlockIndex(const char* name) const;
    virtual void Set(const char* name, const glm::vec2& value) const;
    virtual void Set(const char* name, const glm::vec3& value) const;
    virtual void Set(const char* name, const glm::vec4& value) const;
    virtual void Set(const char* name, const glm::mat4& value, const bool transpose = GL_FALSE) const;
    virtual void Set(const char* name, const float value) const;
    virtual void Set(const char* name, const int32_t value) const;
    virtual void Set(const char* name, const uint32_t value) const;

    inline void Set(const char* name, const bool value) const {
      return Set(name, static_cast<int32_t>(value));
    }

    virtual void Apply();
    virtual void Unapply();
  };
}

#endif //MCC_PROGRAM_SCOPE_APPLY_H