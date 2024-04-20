#ifndef MCC_PROGRAM_SPEC_H
#error "Please #include <mcc/program/program_spec.h> instead."
#endif //MCC_PROGRAM_SPEC_H

#ifndef MCC_PROGRAM_SPEC_BUILDER_H
#define MCC_PROGRAM_SPEC_BUILDER_H

#include "mcc/uri.h"
#include "mcc/shader/shader_id.h"

namespace mcc::program {
  class ProgramSpecBuilder {
    typedef std::set<ShaderId> ShaderSet;

    class ProgramSpecImpl : public ProgramSpec {
      friend class ProgramSpecBuilder;
    protected:
      std::string name_;
      ShaderSet shaders_;

      ProgramSpecImpl(const std::string& name,
                      const ShaderSet& shaders):
        ProgramSpec(),
        name_(name),
        shaders_(shaders) {
      }
    public:
      ~ProgramSpecImpl() override = default;

      std::string GetProgramName() const override {
        return name_;
      }

      rx::observable<ShaderId> GetProgramShaders() const override {
        return rx::observable<>::iterate(shaders_);
      }
    };
  private:
    std::string name_;
    ShaderSet shaders_;
  public:
    explicit ProgramSpecBuilder(const std::string& name):
      name_(name),
      shaders_() {
    }
    explicit ProgramSpecBuilder(const uri::Uri& uri);
    ~ProgramSpecBuilder() = default;

    void Attach(const ShaderId shader) {
      MCC_ASSERT(shader::IsValidShaderId(shader));
      shaders_.insert(shader);
    }

    void Attach(const uri::Uri& shader);
    
    inline void Attach(const uri::basic_uri& shader) {
      auto target_uri = shader;
      if(!StartsWith(target_uri, "file:"))
        target_uri = fmt::format("file:{0:s}", target_uri);
      return Attach(uri::Uri(target_uri));
    }

    void Attach(rx::observable<ShaderId> shaders) {
      shaders.subscribe([this](const ShaderId& next) {
        return Attach(next);
      });
    }

    ProgramSpec* Build() const {
      DLOG(INFO) << "building ProgramSpec " << name_ << " w/ " << shaders_.size() << " shaders....";
      return new ProgramSpecImpl(name_, shaders_);
    }
  };
}

#endif //MCC_PROGRAM_SPEC_BUILDER_H