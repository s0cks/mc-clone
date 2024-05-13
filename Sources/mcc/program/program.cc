#include "mcc/program/program.h"

#include "mcc/json_schema.h"
#include "mcc/shader/shader_code.h"
#include "mcc/shader/shader_compiler.h"
#include "mcc/program/program_builder.h"

namespace mcc::program {
  static rx::subject<ProgramEvent*> events_;

  rx::observable<ProgramEvent*> OnProgramEvent() {
    return events_.get_observable();
  }

  void Program::Publish(ProgramEvent* event) {
    MCC_ASSERT(event);
    const auto& subscriber = events_.get_subscriber();
    subscriber.on_next(event);
  }

  void Program::UseProgram(const ProgramId id) {
    MCC_ASSERT(IsValidProgramId(id) || id == 0);
    glUseProgram(id);
    CHECK_GL(FATAL);
  }

  void Program::DeleteProgram(const ProgramId id) {
    MCC_ASSERT(IsValidProgramId(id) && id != 0); //TODO: better assertion
    glDeleteProgram(id);
    CHECK_GL(FATAL);
  }

  Program::Program(const ProgramId id):
    res::ResourceTemplate<res::kProgramType>(),
    id_(id) {
    Publish<ProgramCreatedEvent>();
  }

  void Program::Destroy() {
    glDeleteProgram(id_);
    CHECK_GL(FATAL);
    Publish<ProgramDestroyedEvent>();
  }

  int Program::GetProgramiv(const Property property) const {
    GLint value;
    glGetProgramiv(GetProgramId(), property, &value);
    return static_cast<int>(value);
  }

  rx::observable<Program::ActiveAttribute> Program::GetActiveAttributes() const {
    return rx::observable<>::create<ActiveAttribute>([this](rx::subscriber<ActiveAttribute> s) {
      GLint length;
      GLint size;
      GLenum type;
      const auto max_length = GetActiveAttributesMaxLength();
      char name[max_length];
      const auto num_attrs = GetNumberOfActiveAttributes();
      for(auto idx = 0; idx < num_attrs; idx++) {
        glGetActiveAttrib(GetProgramId(), idx, max_length, &length, &size, &type, (GLchar*) name);
        CHECK_GL(FATAL);

        const auto attr = ActiveAttribute(type, size, name, length);
        memset(name, 0, sizeof(name));
        s.on_next(attr);
      }
      s.on_completed();
    });
  }

  rx::observable<Program::ActiveUniform> Program::GetActiveUniforms() const {
    return rx::observable<>::create<ActiveUniform>([this](rx::subscriber<ActiveUniform> s) {
      GLint length;
      GLint size;
      GLenum type;
      const auto max_length = GetActiveUniformsMaxLength();
      char name[max_length];
      const auto num_attrs = GetNumberOfActiveUniforms();
      for(auto idx = 0; idx < num_attrs; idx++) {
        glGetActiveUniform(GetProgramId(), idx, max_length, &length, &size, &type, (GLchar*) name);
        CHECK_GL(FATAL);

        const auto attr = ActiveUniform(type, size, name, length);
        memset(name, 0, sizeof(name));
        s.on_next(attr);
      }
      s.on_completed();
    });
  }

  void Program::Apply() {
    glUseProgram(id_);
    CHECK_GL(FATAL);
  }

  void Program::Unapply() {
    glUseProgram(kInvalidProgramId);
    CHECK_GL(FATAL);
  }

  std::string Program::ToString() const {
    std::stringstream ss;
    ss << "Program(";
    ss << "id=" << GetProgramId();
    ss << ")";
    return ss.str();
  }

  static inline std::string
  GetProgramNameFromDirectory(const std::string& dir) {
    const auto slashpos = dir.find_last_of('/');
    if(slashpos == std::string::npos)
      return {};
    return {};
  }

  Program* Program::FromJson(const uri::Uri& uri) {
    if(uri.HasScheme("program")) {
      const auto programs_dir = fmt::format("{0:s}/shaders/", FLAGS_resources);

      auto path = uri.path;
      if(!StartsWith(path, programs_dir))
        path = fmt::format("{0:s}/{1:s}", programs_dir, path[0] == '/' ? path.substr(1) : path);
      
      if(!EndsWith(path, ".json"))
        path = fmt::format("{0:s}.json", path);
      const auto new_uri = uri::Uri(fmt::format("file://{0:s}", path));
      return FromJson(new_uri);
    } else if(!uri.HasScheme("file")) {
      LOG(ERROR) << "invalid Program uri: " << uri;
      return nullptr;
    }
    MCC_ASSERT(uri.HasScheme("file"));
    MCC_ASSERT(uri.HasExtension("json"));

    json::Document doc;
    if(!json::ParseJson(uri, doc)) {
      LOG(ERROR) << "failed to parse Program json from: " << uri;
      return nullptr;
    }

    const auto schema = json::GetSchema();
    MCC_ASSERT(schema);
    const auto valid = schema->Validate(doc);
    if(!valid) {
      LOG(ERROR) << "VertexShader json is invalid: " << valid;
      return nullptr;
    }

    const auto& name_prop = doc["name"];
    MCC_ASSERT(name_prop.IsString());
    const auto name = std::string(name_prop.GetString(), name_prop.GetStringLength());
    MCC_ASSERT(!name.empty());
    
    const auto& type_prop = doc["type"];
    MCC_ASSERT(type_prop.IsString());
    const auto type = std::string(type_prop.GetString(), type_prop.GetStringLength());
    if(!EqualsIgnoreCase(type, "program")) {
      LOG(ERROR) << "invalid type " << type << " for VertexShader json.";
      return nullptr;
    }
    const auto& data_prop = doc["data"];
    return FromJson(data_prop);
  }

  Program* Program::FromJson(const json::Value& value) {
    if(!value.IsObject()) {
      LOG(ERROR) << "invalid program json.";
      return nullptr;
    }

    ProgramBuilder builder;

    MCC_ASSERT(value.HasMember("vertex"));
    const auto& vertex = value["vertex"];
    const auto vertex_shader = VertexShader::FromJson(vertex);
    if(!vertex_shader) {
      LOG(ERROR) << "failed to load vertex shader from program json.";
      return nullptr;
    }
    builder.Attach(vertex_shader);


    MCC_ASSERT(value.HasMember("fragment"));
    const auto& fragment = value["fragment"];
    const auto fragment_shader = FragmentShader::FromJson(fragment);
    if(!fragment_shader) {
      LOG(ERROR) << "failed to load fragment shader from program json.";
      return nullptr;
    }
    builder.Attach(fragment_shader);

    return builder.Build();
  }
}