#include "mcc/program/program_loader.h"

namespace mcc::program {
  rx::observable<Program*> ProgramLoader::LoadProgramAsync() const {
    return rx::observable<>::create<Program*>([this](rx::subscriber<Program*> s) {
      const auto program = LoadProgram();
      if(!program) {
        const auto err = "failed to load Program.";
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
      }

      s.on_next(program);
      s.on_completed();
    });
  }
}