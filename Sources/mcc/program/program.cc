#include "mcc/program/program.h"

#include "mcc/buffer.h"
#include "mcc/json_schema.h"

#include "mcc/shader/shader_code.h"
#include "mcc/shader/shader_compiler.h"

#include "mcc/program/program_json.h"
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

  void Program::Attach(const ProgramId programId, const ShaderId shaderId) {
    glAttachShader(programId, shaderId);
    CHECK_GL(FATAL);
  }

  void Program::Detach(const ProgramId programId, const ShaderId shaderId) {
    glDetachShader(programId, shaderId);
    CHECK_GL(FATAL);
  }

  Program::Program(const Metadata& meta, const ProgramId id):
    gfx::ObjectTemplate<ProgramId>(id) {
    SetMeta(meta);
    Publish<ProgramCreatedEvent>();
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

  std::string Program::ToString() const {
    std::stringstream ss;
    ss << "Program(";
    ss << "meta=" << GetMeta() << ", ";
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

    ProgramReaderHandler handler;
    const auto result = json::ParseJson<ProgramReaderHandler>(uri, handler);
    if(!result) {
      LOG(ERROR) << result;
      return nullptr;
    }

    ProgramBuilder builder;
    builder.SetMeta(handler.GetMeta());

    const auto& shaders = handler.shaders();
    DLOG(INFO) << "shaders:";
    for(const auto& shader : shaders) {
      DLOG(INFO) << " - " << shader;
      switch(shader.GetType()) {
        case shader::kVertexShader: {
          const auto sh = VertexShader::FromJson(shader.GetUri());
          builder.Attach(sh);
          break;
        }
        case shader::kFragmentShader: {
          const auto sh = FragmentShader::FromJson(shader.GetUri());
          builder.Attach(sh);
          break;
        }
      }
    }

    return builder.Build();
  }
}