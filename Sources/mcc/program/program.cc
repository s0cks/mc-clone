#include "mcc/program/program.h"
#include "mcc/shader/shader_code.h"
#include "mcc/shader/shader_compiler.h"
#include "mcc/program/program_loader_dir.h"

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

  ProgramRef Program::New(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("program"));
    const auto base_path = uri.ToFileUri(fmt::format("{0:s}/shaders", FLAGS_resources));
    DLOG(INFO) << "loading Program from: " << base_path;
    const auto parent_path = base_path.GetParent();
    DLOG(INFO) << "parent: " << parent_path;
    const auto name = base_path.GetResourceName();
    DirProgramLoader dir(parent_path.path, name);
    const auto program = dir.LoadProgram();
    if(!program) {
      LOG(FATAL) << "failed to load program from: " << uri;
      return {};
    }
    DLOG(INFO) << uri << " => " << program->ToString();
    return ProgramRef(program);
  }
}