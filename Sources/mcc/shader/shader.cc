#include <sstream>
#include <unordered_map>

#include "mcc/flags.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_compiler.h"

#include "mcc/shader/shader_json.h"
#include "mcc/json_spec.h"
#include "mcc/json_schema.h"

namespace mcc::shader {
  const std::set<std::string> VertexShader::kValidExtensions = {
    "json",
    "glsl",
    "vert",
  };
  const std::set<std::string> FragmentShader::kValidExtensions = {
    "json",
    "glsl",
    "frag",
  };
  const std::set<std::string> GeometryShader::kValidExtensions = {
    "json",
    "glsl",
    "geom",
  };
  const std::set<std::string> TessEvalShader::kValidExtensions = {
    "json",
    "glsl",
    "tese",
  };
  const std::set<std::string> TessControlShader::kValidExtensions = {
    "json",
    "glsl",
    "tesc",
  };

  const std::set<std::string>& GetValidFileExtensions() {
    static std::set<std::string> kValidExtensions;
    if(kValidExtensions.empty()) {
      kValidExtensions.insert(std::begin(VertexShader::kValidExtensions), std::end(VertexShader::kValidExtensions));
      kValidExtensions.insert(std::begin(FragmentShader::kValidExtensions), std::end(FragmentShader::kValidExtensions));
      kValidExtensions.insert(std::begin(GeometryShader::kValidExtensions), std::end(GeometryShader::kValidExtensions));
      kValidExtensions.insert(std::begin(TessEvalShader::kValidExtensions), std::end(TessEvalShader::kValidExtensions));
      kValidExtensions.insert(std::begin(TessControlShader::kValidExtensions), std::end(TessControlShader::kValidExtensions));
    }
    return kValidExtensions;
  }

  static rx::subject<ShaderEvent*> events_;
  static ShaderRegistry shaders_(events_);

  rx::observable<ShaderEvent*> OnEvent() {
    return events_.get_observable();
  }

  const ShaderRegistry& GetRegistry() {
    return shaders_;
  }

  void Shader::Publish(ShaderEvent* event) {
    events_.get_subscriber().on_next(event);
  }

  static inline Shader*
  LoadShaderFromJson(const uri::Uri& uri) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  static inline Shader*
  LoadShaderFromJson(const uri::basic_uri& uri) {
    auto target = uri;
    if(!StartsWith(target, "file://"))
      target = fmt::format("file://{0:s}", target);
    return LoadShaderFromJson(uri::Uri(target));
  }

  static inline Shader*
  LoadShaderFromDirectory(const std::string& dir) {
    NOT_IMPLEMENTED(FATAL); //TODO: implement
    return nullptr;
  }

  Shader* Shader::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("shader", "file"));
    const auto uri_path = fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
    const auto extension = uri.GetExtension();
    if(!extension.empty()) {
      if(EqualsIgnoreCase(extension, "json"))
        return LoadShaderFromJson(uri_path);
#define DEFINE_LOAD_BY_EXTENSION(Name, Ext, GlValue)        \
      else if(EqualsIgnoreCase(extension, #Ext))            \
        return Name##Shader::New(uri);
      FOR_EACH_SHADER_TYPE(DEFINE_LOAD_BY_EXTENSION)
#undef DEFINE_LOAD_BY_EXTENSION
    }

    {
      const auto json_path = fmt::format("{0:s}.json", uri_path);
      if(FileExists(json_path))
        return LoadShaderFromJson(json_path);
    }

    if(IsDirectory(uri_path))
      return LoadShaderFromDirectory(uri_path);

#define DEFINE_LOAD_FUZZY_EXT(Name, Ext, GlValue)                                 \
    {                                                                             \
      const auto glsl_path = fmt::format("{0:s}.{1:s}", uri_path, #Ext);          \
      if(FileExists(glsl_path))                                                   \
        return Name##Shader::New(fmt::format("file://{0:s}", glsl_path));         \
    }
    FOR_EACH_SHADER_TYPE(DEFINE_LOAD_FUZZY_EXT)
#undef DEFINE_LOAD_FUZZY_EXT
    
    LOG(ERROR) << "invalid Shader uri: " << uri;
    return nullptr;
  }

#define DEFINE_NEW_SHADER(Name, Ext, GlValue)                         \
  Name##Shader* Name##Shader::New(const ShaderId id) {                \
    MCC_ASSERT(IsValidShaderId(id));                                  \
    const auto shader = new Name##Shader(id);                         \
    Shader::Publish<ShaderCreatedEvent>(shader);                      \
    return shader;                                                    \
  }                                                                   \
  Name##Shader* Name##Shader::New(ShaderCode* code) {                 \
    MCC_ASSERT(code);                                                 \
    return New(ShaderCompiler::Compile(code));                        \
  }                                                                   \
  Name##Shader* Name##Shader::New(const uri::Uri& uri) {              \
    MCC_ASSERT(uri.HasScheme("shader", "file"));                      \
    MCC_ASSERT(uri.HasExtension(kValidExtensions));                   \
    const auto code = shader::ShaderCode::FromFile(uri);              \
    return code ? New(code) : nullptr;                                \
  }
  FOR_EACH_SHADER_TYPE(DEFINE_NEW_SHADER)
#undef DEFINE_NEW_SHADER

  std::string VertexShader::ToString() const {
    std::stringstream ss;
    ss << "VertexShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string FragmentShader::ToString() const {
    std::stringstream ss;
    ss << "FragmentShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string GeometryShader::ToString() const {
    std::stringstream ss;
    ss << "GeometryShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string TessEvalShader::ToString() const {
    std::stringstream ss;
    ss << "TessEvalShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }

  std::string TessControlShader::ToString() const {
    std::stringstream ss;
    ss << "TessControlShader(";
    ss << "id=" << GetId();
    ss << ")";
    return ss.str();
  }
}