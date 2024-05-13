#ifndef MCC_PROGRAM_BUILDER_H
#define MCC_PROGRAM_BUILDER_H

#include "mcc/rx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_set.h"
#include "mcc/program/program_id.h"

namespace mcc {
  namespace program {
    class Program;
    class ProgramBuilder {
    protected:
      ShaderSet shaders_;
    public:
      ProgramBuilder() = default;
      virtual ~ProgramBuilder() = default;

      const ShaderSet& GetShaders() const {
        return shaders_;
      }

      virtual bool Attach(const Shader* shader);

      virtual Program* Build() const;
      virtual rx::observable<Program*> BuildAsync() const;
    };
  }
}

#endif //MCC_PROGRAM_BUILDER_H