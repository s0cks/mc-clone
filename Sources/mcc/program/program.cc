#include "mcc/program/program.h"

#include "mcc/program/program_linker.h"

#include "mcc/shader/shader_code.h"
#include "mcc/shader/shader_compiler.h"

namespace mcc::program {
  Program::Program(const ProgramId id):
    res::ResourceTemplate<res::kProgramType>(),
    id_(id) {
  }

  void Program::Destroy() {
    glDeleteProgram(id_);
    CHECK_GL(FATAL);
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

  ProgramRef Program::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("program"));
    DLOG(INFO) << "getting program from " << uri << "....";

    const auto base_path = fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
    const auto json_path = fmt::format("{0:s}.json", base_path);
    if(FileExists(json_path)) {
      DLOG(INFO) << "found .json file: " << json_path;
      return {}; //TODO: load from json file
    }

    // no json, check for vertex & fragment files
    shader::ShaderCompiler compiler;
    const auto vertex_path = fmt::format("{0:s}.vert", base_path);
    if(!FileExists(vertex_path)) {
      DLOG(ERROR) << "couldn't find vertex shader file: " << vertex_path;
      return {};
    }
    const auto vertex_code = shader::ShaderCode::FromFile(shader::kVertexShader, fmt::format("file://{0:s}", vertex_path));
    if(!vertex_code || vertex_code->IsEmpty()) {
      DLOG(ERROR) << "failed to get vertex shader code from file: " << vertex_path;
      return {};
    }
    const auto vertex_shader = compiler.Compile(vertex_code);
    if(!shader::IsValidShaderId(vertex_shader)) {
      DLOG(ERROR) << "failed to compile vertex shader code: " << std::endl << (*vertex_code);
      return {};
    }

    const auto fragment_path = fmt::format("{0:s}.frag", base_path);
    if(!FileExists(fragment_path)) {
      DLOG(ERROR) << "couldn't find fragment shader file: " << fragment_path;
      return {};
    }
    const auto fragment_code = shader::ShaderCode::FromFile(shader::kFragmentShader, fmt::format("file://{0:s}", fragment_path));
    if(!fragment_code || fragment_code->IsEmpty()) {
      DLOG(ERROR) << "couldn't get fragment shader file: " << fragment_code;
      return {};
    }
    const auto fragment_shader = compiler.Compile(fragment_code);
    if(!shader::IsValidShaderId(fragment_shader)) {
      DLOG(ERROR) << "failed to compile fragment shader code: " << std::endl << (*fragment_code);
      return {};
    }

    const auto id = glCreateProgram();
    CHECK_GL(FATAL);
    if(!IsValidProgramId(id)) {
      DLOG(ERROR) << "failed to create program.";
      return {};
    }
    
    ProgramLinker linker(id);
    linker.Attach(vertex_shader);
    linker.Attach(fragment_shader);
    const auto link_status = linker.LinkProgram();
    if(!link_status) {
      DLOG(ERROR) << "failed to link program " << id << ": " << link_status;
      return {};
    }
    return New(id);
  }
}