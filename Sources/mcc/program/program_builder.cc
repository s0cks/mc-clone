#include "mcc/program/program_builder.h"

#include <glog/logging.h>

#include "mcc/common.h"
#include "mcc/shader/shader.h"
#include "mcc/program/program.h"
#include "mcc/program/program_status.h"

namespace mcc::program {
  bool ProgramBuilder::Attach(const Shader* shader) {
    MCC_ASSERT(shader);
    const auto inserted = shaders_.Insert(shader);
    if(!inserted) {
      LOG(ERROR) << "failed to insert: " << shader->ToString();
      return false;
    }
    DLOG(INFO) << shader->ToString() << " inserted.";
    return true;
  }

  class ProgramLinkScope {
    typedef std::vector<ShaderId> ShaderList;
  protected:
    ProgramId id_;
    ShaderList linked_;

    inline void Attach(const ShaderId id) {
      Program::Attach(id_, id);
      return linked_.push_back(id);
    }

    inline void Detach(const ShaderId id) {
      return Program::Detach(id_, id);
    }
  public:
    explicit ProgramLinkScope(const ProgramId id, const ShaderSet& shaders):
      id_(id),
      linked_() {
      linked_.reserve(2);
      shaders.ToObservable()
        .as_blocking()
        .subscribe([this](const ShaderId& id) {
          return Attach(id);
        });
    }
    ~ProgramLinkScope() {
      std::for_each(begin(), end(), [this](const ShaderId& id) {
        return Detach(id);
      });
    }

    ProgramId GetProgramId() const {
      return id_;
    }

    ProgramLinkStatus Link() const {
      glLinkProgram(id_);
      CHECK_GL(FATAL);
      return ProgramLinkStatus(id_);
    }

    ShaderList::const_iterator begin() const {
      return std::begin(linked_);
    }

    ShaderList::const_iterator end() const {
      return std::end(linked_);
    }

    ProgramLinkScope& operator<<(const ShaderId id) {
      Program::Attach(id_, id);
      linked_.push_back(id);
      return *this;
    }

    ProgramLinkScope& operator<<(const ShaderList& shaders) {
      if(!shaders.empty())
        linked_.insert(std::end(linked_), std::begin(shaders), std::end(shaders));
      return *this;
    }
  };

  Program* ProgramBuilder::Build() const { //TODO: code cleanup
    // create program
    const auto id = glCreateProgram();
    DLOG(INFO) << "created program: " << id;
    CHECK_GL(FATAL);
    if(!IsValidProgramId(id)) {
      DLOG(ERROR) << "failed to create program.";
      return nullptr;
    }

    ProgramLinkScope link_scope(id, GetShaders());
    const auto status = link_scope.Link();
    if(!status) {
      DLOG(ERROR) << "failed to link Program #" << id << ": " << status;
      return nullptr; //TODO: cleanup
    }

    const auto program = new Program(meta_, id);
#ifdef MCC_DEBUG
    DLOG(INFO) << "attributes: ";
    program->GetActiveAttributes()
      .as_blocking()
      .subscribe([](const Program::ActiveAttribute& attr) {
        DLOG(INFO) << " - " << attr;
      });
    DLOG(INFO) << "uniforms: ";
    program->GetActiveUniforms()
      .as_blocking()
      .subscribe([](const ProgramUniform& uniform) {
        DLOG(INFO) << " - " << uniform;
      });
#endif //MCC_DEBUG
    return program;    
  }

  rx::observable<Program*> ProgramBuilder::BuildAsync() const {
    return rx::observable<>::create<Program*>([this](rx::subscriber<Program*> s) {
      const auto program = Build();
      if(!program) {
        const auto err = "failed to build Program.";
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }
      s.on_next(program);
      s.on_completed();
    });
  }
}