#ifndef MCC_SHADER_H
#define MCC_SHADER_H

#include "mcc/uri.h"
#include "mcc/reference.h"
#include "mcc/resource/resource.h"

#include "mcc/shader/shader_id.h"
#include "mcc/shader/shader_type.h"
#include "mcc/shader/shader_events.h"

namespace mcc {
  namespace shader {
    class Shader;
  }
  using shader::Shader;

  namespace resource {
    typedef Reference<Shader> ShaderRef;
  }
  using res::ShaderRef;

  namespace shader {
#define FOR_EACH_SHADER_PROPERTY(V)                 \
  V(Type, GL_SHADER_TYPE)                           \
  V(SourceLength, GL_SHADER_SOURCE_LENGTH)

    rx::observable<ShaderEvent*> OnEvent();
    
    static inline rx::observable<ShaderEvent*>
    OnEvent(const ShaderId id) {
      return OnEvent()
        .filter(ShaderEvent::FilterBy(id));
    }

#define DEFINE_ON_SHADER_EVENT(Name)                              \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event() {                                           \
      return OnEvent()                                            \
        .filter(Name##Event::Filter)                              \
        .map(Name##Event::Cast);                                  \
    }                                                             \
    static inline rx::observable<Name##Event*>                    \
    On##Name##Event(const ShaderId id) {                          \
      return OnEvent()                                            \
        .filter(Name##Event::FilterBy(id))                        \
        .map(Name##Event::Cast);                                  \
    }
    FOR_EACH_SHADER_EVENT(DEFINE_ON_SHADER_EVENT)
#undef DEFINE_ON_SHADER_EVENT

    class ShaderCompiler;
    class Shader : public res::ResourceTemplate<res::kShaderType> {
      friend class ShaderCompiler;
    private:
      enum Property : GLenum {
#define DEFINE_SHADER_PROPERTY(Name, GlValue) k##Name##Property = (GlValue),
        FOR_EACH_SHADER_PROPERTY(DEFINE_SHADER_PROPERTY)
#undef DEFINE_SHADER_PROPERTY
      };

      static void Publish(ShaderEvent* event);

      template<typename E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish(&event);
      }
    protected:
      ShaderId id_;

      explicit Shader(const ShaderId id);
      int GetShaderiv(const Property property) const;

      void Destroy() override;
    public:
      ~Shader() override = default;//TODO: delete

      virtual ShaderId GetId() const {
        return id_;
      }

      virtual int GetSourceLength() const {
        return GetShaderiv(kSourceLengthProperty);
      }

      virtual ShaderType GetType() const {
        return static_cast<ShaderType>(GetShaderiv(kTypeProperty));
      }

      inline rx::observable<ShaderEvent*> OnEvent() const {
        return shader::OnEvent(GetId());
      }

#define DEFINE_ON_SHADER_EVENT(Name)                                  \
      inline rx::observable<Name##Event*> On##Name##Event() const {   \
        return shader::On##Name##Event(GetId());                      \
      }
      FOR_EACH_SHADER_EVENT(DEFINE_ON_SHADER_EVENT)
#undef DEFINE_ON_SHADER_EVENT

      std::string ToString() const;
    public:
      static inline ShaderRef
      New(const ShaderId id) {
        return ShaderRef(new Shader(id));
      }

      static ShaderRef New(const uri::Uri& uri);

      static inline ShaderRef
      New(const uri::basic_uri& uri) {
        return New(uri::Uri(uri));
      }
    };
  }
}

#endif //MCC_SHADER_H