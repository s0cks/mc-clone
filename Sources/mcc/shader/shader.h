#ifndef MCC_SHADER_H
#define MCC_SHADER_H

#include <glog/logging.h>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/common.h"

#include "mcc/uri.h"
#include "mcc/cache.h"
#include "mcc/resource/resource.h"
#include "mcc/shader/shader_constants.h"

namespace mcc::shader {
  static constexpr const char* kDefaultShaderCacheDir = "";
  DECLARE_string(shader_cache_dir);

  static constexpr const uint64_t kDefaultShaderCacheSize = 128;
  DECLARE_uint32(shader_cache_size);

  static constexpr const uint32_t kDefaultShaderCacheBuckets = 10;
  DECLARE_uint32(shader_cache_buckets);

  static constexpr const char* kVertexShaderExtension = ".vsh";
  static constexpr const char* kFragmentShaderExtension = ".fsh";

  class Shader {
  protected:
    ShaderId id_;
  public:
    Shader(const ShaderId id = kUnknownShaderId):
      id_(id) {
    }
    Shader(const Shader& rhs):
      id_(rhs.id_) {
    }
    virtual ~Shader() = default;

    bool IsCompiled() const {
      return GetShaderId() != kUnknownShaderId;
    }

    ShaderId GetShaderId() const {
      return id_;
    }

    virtual GLint GetUniformLocation(const std::string& name) const {
      return glGetUniformLocation(GetShaderId(), name.c_str());
    }

    virtual GLint GetUniformBlockIndex(const std::string& name) const {
      return glGetUniformBlockIndex(GetShaderId(), name.c_str());
    }

    virtual void SetVec4(const std::string& name, const glm::vec4 value) const {
      glUniform4fv(GetUniformLocation(name), 1, &value[0]);
      CHECK_GL(FATAL);
    }

    virtual void SetVec3(const std::string& name, const glm::vec3 value) const {
      glUniform3fv(GetUniformLocation(name), 1, &value[0]);
      CHECK_GL(FATAL);
    }

    virtual void SetMat4(const std::string& name, const glm::mat4 value) const {
      glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
      CHECK_GL(FATAL);
    }

    virtual void SetFloat(const std::string& name, const float& value) const {
      glUniform1f(GetUniformLocation(name), value);
      CHECK_GL(FATAL);
    }

    virtual void SetInt(const std::string& name, const GLint value) const {
      glUniform1i(GetUniformLocation(name), value);
      CHECK_GL(FATAL);
    }

    virtual inline void SetBool(const std::string& name, const bool value = true) const {
      return SetInt(name, value ? 1 : 0);
    }

    virtual void SetUniformBlock(const std::string& name, const GLuint binding) const {
      glUniformBlockBinding(id_, GetUniformBlockIndex(name), binding);
      CHECK_GL(FATAL);
    }

    virtual void ApplyShader() const {
      glUseProgram(id_);
      CHECK_GL(FATAL);
    }

    virtual void DeleteShader() const {
      glDeleteShader(id_);
      CHECK_GL(FATAL);
    }

    virtual void SetCamera(const std::string& name, const glm::vec3 pos) const {
      SetVec3(name + ".pos", pos);
    }

    virtual void SetLight(const std::string& name, const glm::vec3 pos, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular) const {
      SetVec3(name + ".pos", pos);
      SetVec3(name + ".ambient", ambient);
      SetVec3(name + ".diffuse", diffuse);
      SetVec3(name + ".specular", specular);
    }

    virtual void SetMaterial(const std::string& name) const {
      SetInt(name + ".albedo", 0);
      SetInt(name + ".ao", 1);
      SetInt(name + ".height", 2);
      SetInt(name + ".metallic", 3);
      SetInt(name + ".normal", 4);
      SetInt(name + ".roughness", 5);
    }

    Shader& operator=(const Shader& rhs) {
      if(id_ == rhs.id_)
        return *this;
      id_ = rhs.id_;
      return *this;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Shader rhs) {
      stream << "Shader(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
}

namespace mcc {
  using shader::Shader;
  using shader::ShaderId;
  using shader::kUnknownShaderId;

  namespace resource {
    typedef Reference<Shader> ShaderRef;
  }

  using resource::ShaderRef;

  ShaderRef GetShader(const uri::Uri& uri);
  
  static inline ShaderRef
  GetShader(const uri::basic_uri& uri) {
    return GetShader(uri::Uri(uri));
  }
}

#endif //MCC_SHADER_H