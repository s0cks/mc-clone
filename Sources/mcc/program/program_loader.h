#ifndef MCC_PROGRAM_LOADER_H
#define MCC_PROGRAM_LOADER_H

#include "mcc/rx.h"

namespace mcc::program {
  class Program;
  class ProgramLoader {
  public:
    ProgramLoader() = default;
    virtual ~ProgramLoader() = default;
    virtual Program* LoadProgram() const = 0;
    virtual rx::observable<Program*> LoadProgramAsync() const;
  };
}

#endif //MCC_PROGRAM_LOADER_H