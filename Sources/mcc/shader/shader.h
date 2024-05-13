#ifndef MCC_SHADER_H
#define MCC_SHADER_H

#include <set>
#include <optional>
#include "mcc/uri.h"
#include "mcc/reference.h"
#include "mcc/resource/resource.h"

#include "mcc/shader/shader_id.h"
#include "mcc/shader/shader_code.h"
#include "mcc/shader/shader_type.h"
#include "mcc/shader/shader_events.h"
#include "mcc/shader/shader_registry.h"

namespace mcc {
  namespace shader {
    class Shader;
  }
  using shader::Shader;
#define DEFINE_USE_SHADER_TYPE(Name, Ext, GlValue)      \
  using shader::Name##Shader;
  FOR_EACH_SHADER_TYPE(DEFINE_USE_SHADER_TYPE)
#undef DEFINE_USE_SHADER_TYPE

  namespace resource {
    typedef Reference<Shader> ShaderRef;
  }
  using res::ShaderRef;

  namespace shader {
    const std::set<std::string>& GetValidFileExtensions();
    const ShaderRegistry& GetRegistry();
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

    class ShaderCode;
    class ShaderCompiler;
    class Shader {
      friend class ShaderCompiler;
      friend class VertexShader;
      friend class FragmentShader;
      friend class GeometryShader;
      friend class TessEvalShader;
      friend class TessControlShader;
    public:
      struct Comparator {
        bool operator()(const Shader* lhs,
                        const Shader* rhs) const {
          return lhs->GetId() == rhs->GetId()
              && lhs->GetType() == rhs->GetType();
        }
      };
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
      virtual ShaderType GetType() const = 0;
      virtual std::string ToString() const = 0;
      
      ShaderId GetId() const {
        return id_;
      }

      virtual bool Equals(const Shader* rhs) const {
        return GetType() == rhs->GetType()
            && GetId() == rhs->GetId();
      }

#define DEFINE_TYPE_CHECK(Name, Ext, GlValue)                             \
      virtual Name##Shader* As##Name##Shader() { return nullptr; }        \
      bool Is##Name##Shader() { return As##Name##Shader() != nullptr; }
      FOR_EACH_SHADER_TYPE(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK

      rx::observable<ShaderEvent*> OnEvent() const {
        return shader::OnEvent(GetId());
      }

#define DEFINE_ON_SHADER_EVENT(Name)                                  \
      rx::observable<Name##Event*> On##Name##Event() const {          \
        return shader::On##Name##Event(GetId());                      \
      }
      FOR_EACH_SHADER_EVENT(DEFINE_ON_SHADER_EVENT)
#undef DEFINE_ON_SHADER_EVENT
    public:
      static Shader* New(const uri::Uri& uri);
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
    };

#define DECLARE_SHADER_TYPE(Name)                                   \
    public:                                                         \
      std::string ToString() const override;                        \
      Name##Shader* As##Name##Shader() override { return this; }    \
    private:                                                        \
      static Name##Shader* New(const ShaderId id);                  \
    public:                                                         \
      static const std::set<std::string> kValidExtensions;          \
      static Name##Shader* New(ShaderCode* code);                   \
      static Name##Shader* New(const uri::Uri& uri);                \
      static inline Name##Shader*                                   \
      New(const uri::basic_uri& uri) {                              \
        return New(uri::Uri(uri));                                  \
      }

    class VertexShader : public ShaderTemplate<kVertexShader> {
    protected:
      explicit VertexShader(const ShaderId id):
        ShaderTemplate<kVertexShader>(id) {  
      }
    public:
      ~VertexShader() override = default;
      DECLARE_SHADER_TYPE(Vertex);
    public:
      static VertexShader* FromSource(ShaderCode* code);
      static VertexShader* FromSource(const uri::Uri& uri);
      static VertexShader* FromJson(const uri::Uri& uri);
      static VertexShader* FromJson(const json::Value& value);
      static VertexShader* FromJson(const char* source, const uword length);

      static inline VertexShader*
      FromJson(const char* source) {
        return FromJson(source, strlen(source));
      }

      static inline VertexShader*
      FromJson(const std::string& source) {
        return FromJson(source.data(), source.length());
      }

      static inline VertexShader*
      FromSource(const uint8_t* bytes, const uword num_bytes) {
        return FromSource(ShaderCode::NewVertexShaderCode(bytes, num_bytes));
      }

      static inline VertexShader*
      FromSource(const char* source, const uword length) {
        if(StartsWith(source, length, "file:")
        || StartsWith(source, length, "shader:")) {
          return FromSource(uri::Uri(std::string(source, length)));
        }
        return FromSource(ShaderCode::NewVertexShaderCode(source, length));
      }

      static inline VertexShader*
      FromSource(const char* source) {
        return FromSource(source, strlen(source));
      }

      static inline VertexShader*
      FromSource(const std::string& source) {
        return FromSource(source.data(), source.length());
      }
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