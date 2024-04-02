#include "mcc/shader/shader_source_resolver.h"

#include <optional>
#include "mcc/flags.h"
#include "mcc/common.h"

#include "mcc/shader/shader_document.h"

namespace mcc::shader {
  ShaderSourceResolver::ShaderSourceResolver(const std::string& dir, const uri::Uri& target):
    dir_(dir),
    target_(target) {  
  }

  ShaderSourceResolver::ShaderSourceResolver(const uri::Uri& target):
    ShaderSourceResolver(FLAGS_resources, target) {
  }

  static inline std::string
  GetShaderPath(const uri::Uri& uri) {
    return fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources);
  }

  static inline std::optional<std::string>
  GetFileExtension(const std::string& filename) {
    const auto dotpos = filename.find_first_of('.');
    if(dotpos == std::string::npos)
      return std::nullopt;
    return { filename.substr(dotpos + 1) };
  }

  static inline bool
  IsJsonFile(const std::string& filename) {
    //TODO: stat for file type
    const auto ext = GetFileExtension(filename);
    return ext
        && (EqualsIgnoreCase((*ext), "json")
        || EqualsIgnoreCase((*ext), "json5"));
  }

  static inline bool
  ShaderSourceExists(const std::string& prefix, const ShaderType type) {
    switch(type) {
#define SHADER_SOURCE_EXISTS(Name, Ext, GlValue) \
      case ShaderType::k##Name##Shader: return FileExists(fmt::format("{0:s}.{1:s}", prefix, (#Ext)));
      FOR_EACH_SHADER_TYPE(SHADER_SOURCE_EXISTS)
#undef SHADER_SOURCE_EXISTS
      default: return false;
    }
  }

  rx::observable<ShaderSource> ShaderSourceResolver::ResolveFromJsonFile(const std::string& jsonf) {
    //TODO: parse json
    return rx::observable<>::error<ShaderSource>(std::runtime_error("Hello World"));
  }

  rx::observable<ShaderSource> ShaderSourceResolver::Resolve() {
    MCC_ASSERT(target().HasScheme("shader"));
    // check if exists
    DLOG(INFO) << "loading " << target() << " from: " << dir_;
    const auto path = fmt::format("{0:s}/shaders/{1:s}", dir_, target().path);
    DLOG(INFO) << "path: " << path;
    // check for directory
    if(IsDirectory(path)) {
      // check for .json in dir
      // check for .frag & .vert in dir
    }

    // check for .json
    if(FileExists(fmt::format("{0:s}.json", path))) {
      DLOG(INFO) << "found json: " << fmt::format("{0:s}.json", path);
    }

    DLOG(INFO) << "scanning....";
    // check for files manually
    return rx::observable<>::create<ShaderSource>([this,path](rx::subscriber<ShaderSource> s) {
      DLOG(INFO) << "looking for shader sources: " << path;
      const auto fragPath = fmt::format("{0:s}.{1:s}", path, GetExtensionForType(kFragmentShader));
      if(!FileExists(fragPath)) {
        DLOG(ERROR) << "cannot find fragment shader for: " << fragPath;
        s.on_error(std::make_exception_ptr(std::runtime_error(fmt::format("cannot find fragment shader: {0:s}", fragPath))));
        return;
      }
      ShaderSource fragSource(kFragmentShader, fragPath);
      DLOG(INFO) << "found: " << fragSource;
      s.on_next(fragSource);

      const auto vertPath = fmt::format("{0:s}.{1:s}", path, GetExtensionForType(kVertexShader));
      if(!FileExists(vertPath)) {
        DLOG(ERROR) << "cannot find vertex shader for: " << vertPath;
        s.on_error(std::make_exception_ptr(std::runtime_error(fmt::format("cannot find vertex shader for: {0:s}", path))));
        return;
      }
      ShaderSource vertSource(kVertexShader, vertPath);
      DLOG(INFO) << "found: " << vertSource;
      s.on_next(vertSource);

      s.on_completed();
    });
  }
}