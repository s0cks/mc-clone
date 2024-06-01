#ifndef MCC_PROGRAM_BUILDER_H
#define MCC_PROGRAM_BUILDER_H

#include "mcc/rx.h"
#include "mcc/builder.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_set.h"
#include "mcc/program/program_id.h"

namespace mcc {
  namespace program {
    class Program;
    class ProgramBuilder : public BuilderTemplate<Program> {
    protected:
      ShaderSet shaders_;
    public:
      ProgramBuilder() = default;
      ~ProgramBuilder() override = default;

      const ShaderSet& GetShaders() const {
        return shaders_;
      }

      virtual bool Attach(const Shader* shader);

      Program* Build() const override;
      rx::observable<Program*> BuildAsync() const override;
    };
  }
}

#endif //MCC_PROGRAM_BUILDER_H