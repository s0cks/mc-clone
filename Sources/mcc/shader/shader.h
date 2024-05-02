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
    rx::observable<ShaderEvent*> OnEvent();
    
    static inline rx::observable<ShaderEvent*>
    OnEvent(const ShaderId id) {
      return OnEvent()
        .filter([id](ShaderEvent* event) {
          return event
              && event->GetShaderId() == id;
        });
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
    class Shader {
      friend class VertexShader;
      friend class FragmentShader;
      friend class GeometryShader;
      friend class TessEvalShader;
      friend class TessControlShader;
      
      friend class ShaderCompiler;
    private:
      static void Publish(ShaderEvent* event);

      template<typename E, typename... Args>
      static inline void
      Publish(Args... args) {
        E event(args...);
        return Publish(&event);
      }
    protected:
      ShaderId id_;

      explicit Shader(const ShaderId id):
        id_(id) {
      }
    public:
      virtual ~Shader() = default;
      virtual bool Equals(const Shader* rhs) const = 0;
      virtual ShaderType GetType() const = 0;
      virtual std::string ToString() const = 0;
      
      ShaderId GetId() const {
        return id_;
      }

      rx::observable<ShaderEvent*> OnEvent() const {
        return shader::OnEvent(GetId());
      }

#define DEFINE_ON_SHADER_EVENT(Name)                                  \
      rx::observable<Name##Event*> On##Name##Event() const {          \
        return shader::On##Name##Event(GetId());                      \
      }
      FOR_EACH_SHADER_EVENT(DEFINE_ON_SHADER_EVENT)
#undef DEFINE_ON_SHADER_EVENT
    };

    template<const ShaderType Type>
    class ShaderTemplate : public Shader {
    protected:
      explicit ShaderTemplate(const ShaderId id):
        Shader(id) {
      }
    public:
      ~ShaderTemplate() override = default;

      ShaderType GetType() const override {
        return Type;
      }

      bool Equals(const Shader* rhs) const override {
        return GetType() == rhs->GetType()
            && GetId() == rhs->GetId();
      }
    };

#define DECLARE_SHADER_TYPE(Name)                                 \
    public:                                                       \
      std::string ToString() const override;                      \
    private:                                                      \
      static Name##Shader* New(const ShaderId id);                \
    public:                                                       \
      static Name##Shader* New(const uri::Uri& uri);              \
      static inline Name##Shader*                                 \
      New(const uri::basic_uri& uri) {                            \
        return New(uri::Uri(uri));                                \
      }

    class VertexShader : public ShaderTemplate<kVertexShader> {
    protected:
      explicit VertexShader(const ShaderId id):
        ShaderTemplate<kVertexShader>(id) {  
      }
    public:
      ~VertexShader() override = default;
      DECLARE_SHADER_TYPE(Vertex);
    };

    class FragmentShader : public ShaderTemplate<kFragmentShader> {
    protected:
      explicit FragmentShader(const ShaderId id):
        ShaderTemplate<kFragmentShader>(id) {  
      }
    public:
      ~FragmentShader() override = default;
      DECLARE_SHADER_TYPE(Fragment);
    };

    class GeometryShader : public ShaderTemplate<kGeometryShader> {
    protected:
      explicit GeometryShader(const ShaderId id):
        ShaderTemplate<kGeometryShader>(id) {
      }
    public:
      ~GeometryShader() override = default;
      DECLARE_SHADER_TYPE(Geometry);
    };

    class TessEvalShader : public ShaderTemplate<kTessEvalShader> {
    protected:
      explicit TessEvalShader(const ShaderId id):
        ShaderTemplate<kTessEvalShader>(id) {  
      }
    public:
      ~TessEvalShader() override = default;
      DECLARE_SHADER_TYPE(TessEval);
    };

    class TessControlShader : public ShaderTemplate<kTessControlShader> {
    protected:
      explicit TessControlShader(const ShaderId id):
        ShaderTemplate<kTessControlShader>(id) {
      }
    public:
      ~TessControlShader() override = default;
      DECLARE_SHADER_TYPE(TessControl);
    };
#undef DECLARE_SHADER_TYPE
  }
}

#endif //MCC_SHADER_H