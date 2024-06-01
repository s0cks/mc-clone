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

  Program* ProgramBuilder::Build() const { //TODO: code cleanup
    // create program
    const auto id = glCreateProgram();
    CHECK_GL(FATAL);
    if(!IsValidProgramId(id)) {
      DLOG(ERROR) << "failed to create program.";
      return nullptr;
    }

    // attach all...
    const auto& shaders = GetShaders();
    for(const auto& shader : shaders) {
      Program::Attach(id, shader.id());
    }

    // link program
    glLinkProgram(id);
    CHECK_GL(FATAL);

    const auto status = ProgramLinkStatus(id);
    if(!status) {
      DLOG(ERROR) << "failed to link Program #" << id << ": " << status;
      return nullptr; //TODO: cleanup
    }

    //TODO: more cleanup
    for(const auto& shader : GetShaders()) {
      Program::Detach(id, shader.id());
    }
    return new Program(meta_, id);
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