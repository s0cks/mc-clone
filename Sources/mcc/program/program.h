#ifndef MCC_PROGRAM_H
#define MCC_PROGRAM_H

#include "mcc/rx.h"
#include "mcc/reference.h"
#include "mcc/resource/resource.h"

#include "mcc/shader/shader_id.h"
#include "mcc/program/program_id.h"

namespace mcc {
  namespace program {
    class Program;
  }
  using program::Program;

  namespace resource {
    typedef Reference<Program> ProgramRef;
  }
  using res::ProgramRef;

#define FOR_EACH_PROGRAM_PROPERTY(V)                              \
  V(AttachedShaders, GL_ATTACHED_SHADERS)                         \
  V(ActiveAttributes, GL_ACTIVE_ATTRIBUTES)                       \
  V(ActiveAttributesMaxLength, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH)    \
  V(ActiveUniforms, GL_ACTIVE_UNIFORMS)                           \
  V(ActiveUniformsMaxLength, GL_ACTIVE_UNIFORM_MAX_LENGTH)

  namespace program {
    class Program : public res::ResourceTemplate<res::kProgramType> {
    public:
      struct ActiveAttribute {
        GLenum type;
        GLint size;
        std::string name;

        ActiveAttribute() = default;
        ActiveAttribute(const ActiveAttribute& rhs) = default;
        ~ActiveAttribute() = default;
        ActiveAttribute& operator=(const ActiveAttribute& rhs) = default;

        friend class Program;
      private:
        ActiveAttribute(const GLenum t,
                        const GLint s,
                        const char* name,
                        const int name_length):
          type(t),
          size(s),
          name(name, name_length) {
        }
      };

      struct ActiveUniform {
        GLenum type;
        GLint size;
        std::string name;

        ActiveUniform() = default;
        ActiveUniform(const ActiveUniform& rhs) = default;
        ~ActiveUniform() = default;
        ActiveUniform& operator=(const ActiveUniform& rhs) = default;

        friend class Program;
      private:
        ActiveUniform(const GLenum t,
                        const GLint s,
                        const char* name,
                        const int name_length):
          type(t),
          size(s),
          name(name, name_length) {
        }
      };
    protected:
      enum Property : GLenum {
  #define DEFINE_PROGRAM_PROPERTY(Name, GlValue) k##Name = (GlValue),
        FOR_EACH_PROGRAM_PROPERTY(DEFINE_PROGRAM_PROPERTY)
  #undef DEFINE_PROGRAM_PROPERTY
      };
    protected:
      ProgramId id_;

      explicit Program(const ProgramId id);
      void Destroy() override;

      int GetProgramiv(const Property property) const;

      inline int GetActiveAttributesMaxLength() const {
        return GetProgramiv(kActiveAttributesMaxLength);
      }

      inline int GetActiveUniformsMaxLength() const {
        return GetProgramiv(kActiveUniformsMaxLength);
      }
    public:
      virtual ~Program() override = default;

      ProgramId GetProgramId() const {
        return id_;
      }

      virtual int GetNumberOfAttachedShaders() const {
        return GetProgramiv(kAttachedShaders);
      }

      virtual int GetNumberOfActiveAttributes() const {
        return GetProgramiv(kActiveAttributes);
      }

      virtual rx::observable<ActiveAttribute> GetActiveAttributes() const;

      virtual int GetNumberOfActiveUniforms() const {
        return GetProgramiv(kActiveUniforms);
      }

      virtual rx::observable<ActiveUniform> GetActiveUniforms() const;

      virtual GLint GetUniformLocation(const std::string& name) const {
        return glGetUniformLocation(GetProgramId(), name.c_str());
      }

      virtual GLint GetUniformBlockIndex(const std::string& name) const {
        return glGetUniformBlockIndex(GetProgramId(), name.c_str());
      }

      virtual void SetVec4(const std::string& name, const glm::vec4 value) const {
        glUniform4fv(GetUniformLocation(name), 1, &value[0]);
        CHECK_GL(FATAL);
      }

      virtual void SetVec3(const std::string& name, const glm::vec3 value) const {
        glUniform3fv(GetUniformLocation(name), 1, &value[0]);
        CHECK_GL(FATAL);
      }

      virtual void SetMat4(const std::string& name, const glm::mat4 value) const {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
        CHECK_GL(FATAL);
      }

      virtual void SetFloat(const std::string& name, const float& value) const {
        glUniform1f(GetUniformLocation(name), value);
        CHECK_GL(FATAL);
      }

      virtual void SetInt(const std::string& name, const GLint value) const {
        glUniform1i(GetUniformLocation(name), value);
        CHECK_GL(FATAL);
      }

      virtual inline void SetBool(const std::string& name, const bool value = true) const {
        return SetInt(name, value ? 1 : 0);
      }

      virtual void SetUniformBlock(const std::string& name, const GLuint binding) const {
        glUniformBlockBinding(id_, GetUniformBlockIndex(name), binding);
        CHECK_GL(FATAL);
      }

      void Apply();
      void Unapply();
      std::string ToString() const;
    public:
      static inline ProgramRef
      New(const ProgramId id) {
        MCC_ASSERT(IsValidProgramId(id));
        return ProgramRef(new Program(id));
      }

      static ProgramRef New(const uri::Uri& uri);

      static inline ProgramRef
      New(const uri::basic_uri& uri) {
        auto target = uri;
        if(!StartsWith(target, "program:"))
          target = fmt::format("program:{0:s}", target);
        return New(uri::Uri(target));
      }
    };
  }
}

#endif //MCC_PROGRAM_H