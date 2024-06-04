#ifndef MCC_PROGRAM_H
#define MCC_PROGRAM_H

#include "mcc/rx.h"
#include "mcc/reference.h"
#include "mcc/gfx_object.h"
#include "mcc/resource/resource.h"

#include "mcc/shader/shader_id.h"
#include "mcc/program/program_id.h"
#include "mcc/program/program_events.h"
#include "mcc/program/program_uniform.h"

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
    rx::observable<ProgramEvent*> OnProgramEvent();

    static inline rx::observable<ProgramEvent*>
    OnProgramEvent(const ProgramId id) {
      return OnProgramEvent()
        .filter([id](ProgramEvent* event) {
          //TODO: cleanup
          return event
              && event->GetProgramId() == id;
        });
    }

  #define DEFINE_ON_PROGRAM_EVENT(Name)                           \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event() {                                           \
      return OnProgramEvent()                                     \
        .filter(Name##Event::Filter)                              \
        .map(Name##Event::Cast);                                  \
    }                                                             \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event(const ProgramId id) {                         \
      return OnProgramEvent()                                     \
        .filter(Name##Event::FilterBy(id))                        \
        .map(Name##Event::Cast);                                  \
    }
    FOR_EACH_PROGRAM_EVENT(DEFINE_ON_PROGRAM_EVENT)
#undef DEFINE_ON_PROGRAM_EVENT

    class Program : public gfx::ObjectTemplate<ProgramId> {
      friend class ProgramLinker;
      friend class ProgramBuilder;
      friend class ApplyProgramScope;
      friend class ProgramLinkScope;
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

        friend std::ostream& operator<<(std::ostream& stream, const ActiveAttribute& rhs) {
          stream << "ActiveAttribute(";
          stream << "type=" << rhs.type << ", ";
          stream << "size=" << rhs.size << ", ";
          stream << "name=" << rhs.name;
          stream << ")";
          return stream;
        }
      };
    protected:
      enum Property : GLenum {
  #define DEFINE_PROGRAM_PROPERTY(Name, GlValue) k##Name = (GlValue),
        FOR_EACH_PROGRAM_PROPERTY(DEFINE_PROGRAM_PROPERTY)
  #undef DEFINE_PROGRAM_PROPERTY
      };
      
      static void UseProgram(const ProgramId id);
      static void DeleteProgram(const ProgramId id);
      static void Attach(const ProgramId program, const ShaderId shader);
      static void Detach(const ProgramId program, const ShaderId shader);

      static inline void
      UseDefault() {
        return UseProgram(kInvalidProgramId);
      }
    protected:
      explicit Program(const Metadata& meta, const ProgramId id);

      int GetProgramiv(const Property property) const;

      inline int GetActiveAttributesMaxLength() const {
        return GetProgramiv(kActiveAttributesMaxLength);
      }

      inline int GetActiveUniformsMaxLength() const {
        return GetProgramiv(kActiveUniformsMaxLength);
      }

      static void Publish(ProgramEvent* event);

      template<typename E, typename... Args>
      inline void
      Publish(Args... args) const {
        E event(GetId(), args...);
        return Publish(&event);
      }
    public:
      virtual ~Program() override = default;

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

      virtual rx::observable<ProgramUniform> GetActiveUniforms() const;

      virtual GLint GetUniformLocation(const std::string& name) const {
        return glGetUniformLocation(GetId(), name.c_str());
      }

      virtual GLint GetUniformBlockIndex(const std::string& name) const {
        return glGetUniformBlockIndex(GetId(), name.c_str());
      }

      virtual void SetUniformBlock(const std::string& name, const GLuint binding) const {
        glUniformBlockBinding(id_, GetUniformBlockIndex(name), binding);
        CHECK_GL(FATAL);
      }

      std::string ToString() const override;

      inline rx::observable<ProgramEvent*>
      OnEvent() const {
        return OnProgramEvent(GetId());
      }

#define DEFINE_ON_PROGRAM_EVENT(Name)                      \
      inline rx::observable<Name##Event*>                  \
      On##Name##Event() const {                            \
        return program::On##Name##Event(GetId());          \
      }
      FOR_EACH_PROGRAM_EVENT(DEFINE_ON_PROGRAM_EVENT);
#undef DEFINE_ON_PROGRAM_EVENT
    private:
      static inline Program*
      New(const Metadata& meta, const ProgramId id) {
        MCC_ASSERT(IsValidProgramId(id));
        return new Program(meta, id);
      }
    public:
      static inline Program*
      New(const ProgramId id) {
        MCC_ASSERT(IsValidProgramId(id));
        Metadata meta;
        return New(meta, id);
      }

      static Program* FromJson(const uri::Uri& uri);

      static inline Program*
      FromJson(const uri::basic_uri& uri) {
        return FromJson(uri::Uri(uri)); //TODO: sanitize uri -> uri::Uri("file://{0:s}/{1:s}.json", programs_dir, uri.path)
      }
    };
  }
}

#endif //MCC_PROGRAM_H