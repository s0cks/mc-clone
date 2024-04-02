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

  template<const ShaderType Type>
  static inline std::optional<ShaderSource>
  FindShaderSource(const std::string& dir, const std::string& name) {
    DLOG(INFO) << "getting " << Type << " shader source for: " << name << " in " << dir << "....";
    const auto path = fmt::format("{0:s}/{1:s}.{2:s}", dir, name, GetExtensionForType(Type));
    if(!FileExists(path))
      return std::nullopt;
    return { ShaderSource(Type, path) };
  }

  static inline rx::observable<ShaderSource> LoadShaderFromDirectory(const std::string& dir, const std::string& name) {
    return rx::observable<>::create<ShaderSource>([dir,name](rx::subscriber<ShaderSource> s) {
      const auto f = FindShaderSource<kFragmentShader>(dir, name);
      if(!f)
        return s.on_error(std::make_exception_ptr(std::runtime_error(fmt::format("cannot find fragment shader source for {0:s} in {1:s}", name, dir))));
      s.on_next(*f);

      const auto v = FindShaderSource<kVertexShader>(dir, name);
      if(!v)
        return s.on_error(std::make_exception_ptr(std::runtime_error(fmt::format("cannot find vertex shader source for {0:s} in {1:s}", name, dir))));
      s.on_next(*v);

      const auto g = FindShaderSource<kGeometryShader>(dir, name);
      if(g)
        return s.on_next(*g);
      
      const auto tc = FindShaderSource<kTessControlShader>(dir, name);
      if(tc)
        return s.on_next(*tc);

      const auto te = FindShaderSource<kTessEvalShader>(dir, name);
      if(te)
        return s.on_next(*te);
      s.on_completed();
    });
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
    const auto dir = path.substr(0, path.size() - (path.size() - path.find_last_of('/')));
    return LoadShaderFromDirectory(dir, target().path);
  }
}