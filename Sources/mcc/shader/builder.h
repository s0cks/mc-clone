#ifndef MCC_SHADER_LINKER_H
#define MCC_SHADER_LINKER_H

#include "mcc/gfx.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/source.h"

namespace mcc::shader {
  class ProgramBuilder {
  protected:
    ShaderId id_;
  public:
    explicit ProgramBuilder(const ShaderId id = glCreateProgram()):
      id_(id) {
    }
    virtual ~ProgramBuilder() = default;

    ShaderId id() const {
      return id_;
    }

    void Attach(const ShaderId& id) {
      glAttachShader(id_, id);
      CHECK_GL(FATAL);
    }

    void Attach(rx::observable<ShaderId>& shaders) {
      shaders.subscribe([this](const ShaderId& id) {
        return Attach(id);
      });
    }

    ShaderId Build() {
      glLinkProgram(id_);
      CHECK_GL(FATAL);
      GLint success;
      GLchar message[1024];
      glGetProgramiv(id_, GL_LINK_STATUS, &success);
      if(!success) {
        glGetProgramInfoLog(id_, 1024, NULL, message);
        DLOG(INFO) << "failed to compile shader: " << message;
      }
      return id_;
    }
  };
}

#endif//MCC_SHADER_LINKER_H