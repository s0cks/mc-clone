#include "mcc/program/program_spec.h"

#include <fmt/format.h>
#include "mcc/shader/shader_compiler.h"

namespace mcc::program {
  static inline std::string
  GetProgramName(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("program"));
    const auto& path = uri.path;
    const auto slashpos = path.find_last_of('/');
    if(slashpos == std::string::npos)
      return uri.path;
    return path.substr(slashpos + 1);
  }

  static inline void
  ResolveShader(std::set<ShaderId>& shaders, const std::string& path) {
    if(!FileExists(path)) {
      DLOG(WARNING) << "couldn't find shader file: " << path;
      return;
    }
    
    const auto id = shader::ShaderCompiler::Compile(path);
    if(!shader::IsValidShaderId(id)) {
      DLOG(ERROR) << "failed to compile shader file: " << path;
      return;
    }
    shaders.insert(id);
  }

  static inline void
  ResolveShaderFiles(std::set<ShaderId>& shaders, const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme("program"));
    MCC_ASSERT(!uri.HasExtension());

    const auto base_path = fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
    DLOG(INFO) << "resolving shader files for " << uri << "....";
    ResolveShader(shaders, fmt::format("{0:s}.vert", base_path));
    ResolveShader(shaders, fmt::format("{0:s}.frag", base_path));
  }

  ProgramSpecBuilder::ProgramSpecBuilder(const uri::Uri& uri):
    name_(GetProgramName(uri)),
    shaders_() {
    DLOG(INFO) << "creating program builder for " << name_ << ": " << uri;
    ResolveShaderFiles(shaders_, uri);
    DLOG(INFO) << "resolved " << shaders_.size() << " for " << name_ << " (" << uri << ")";
  }

  void ProgramSpecBuilder::Attach(const uri::Uri& uri) {
    const auto id = shader::ShaderCompiler::Compile(uri);
    if(!shader::IsValidShaderId(id)) {
      DLOG(ERROR) << "cannot attach shader from: " << uri;
      return;
    }
    return Attach(id);
  }
}