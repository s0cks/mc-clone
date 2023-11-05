#ifndef MCC_SHADER_H
#define MCC_SHADER_H

#include <glog/logging.h>

#include "mcc/gfx.h"
#include "mcc/flags.h"
#include "mcc/buffer.h"
#include "mcc/common.h"

#include "mcc/cache.h"

namespace mcc::shader {
  static constexpr const char* kDefaultShaderCacheDir = "";
  DECLARE_string(shader_cache_dir);

  static constexpr const uint64_t kDefaultShaderCacheSize = 128;
  DECLARE_uint32(shader_cache_size);

  static constexpr const uint32_t kDefaultShaderCacheBuckets = 10;
  DECLARE_uint32(shader_cache_buckets);

  static constexpr const char* kVertexShaderExtension = ".vsh";
  static constexpr const char* kFragmentShaderExtension = ".fsh";

  typedef GLuint ShaderId;

  static constexpr const ShaderId kUnknownShaderId = 0;

  enum ShaderType : GLenum {
    kCompositeShader = 1,
    kVertexShader = GL_VERTEX_SHADER,
    kFragmentShader = GL_FRAGMENT_SHADER,
    kGeometryShader = GL_GEOMETRY_SHADER,
    kTessEvalShader = GL_TESS_EVALUATION_SHADER,
    kTessControlShader = GL_TESS_CONTROL_SHADER,
  };

  class Shader {
  public:
    typedef Cache<std::string, Shader> Cache;
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

    virtual void ApplyShader() const {
      glUseProgram(id_);
      CHECK_GL(FATAL);
    }

    virtual void DeleteShader() const {
      glDeleteShader(id_);
      CHECK_GL(FATAL);
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
  private:
    static const std::function<Shader(const std::string&)> kDefaultShaderCacheLoader;
  public:
    static inline std::string
    GetCacheDirectory() {
      if(FLAGS_shader_cache_dir.empty())
        return FLAGS_resources + "/shaders";
      return FLAGS_shader_cache_dir;
    }

    static inline uint64_t
    GetCacheCapacity() {
      return FLAGS_shader_cache_size;
    }

    static uint64_t GetCacheSize();
    static Shader Get(const std::string& k, std::function<Shader(const std::string&)> loader);
    
    static inline Shader
    Get(const std::string& name) {
      return Get(name, kDefaultShaderCacheLoader);
    }

    static inline Shader
    GetOrDefault(const std::string& name, const Shader& defaultShader) {
      return Get(name, [&defaultShader](const std::string& name) {
        return defaultShader;
      });
    }
  };

  typedef Cache<std::string, Shader> ShaderCache;
}

#endif //MCC_SHADER_H