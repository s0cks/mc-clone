#include "mcc/shader/shader_resolver.h"
#include "mcc/flags.h"

namespace mcc {
  static inline bool HasExtension(const std::string& path) {
    const auto dotpos = path.find_last_of('.');
    return dotpos != std::string::npos;
  }

  static inline bool GetExtension(const std::string& path, std::string& result) {
    const auto dotpos = path.find_last_of('.');
    if(dotpos != std::string::npos)
      return false;
    result = path.substr(dotpos + 1);
    return true;
  }

  static inline std::string
  NormalizePath(const std::string& root, const uri::Uri& target) {
    auto path = target.path;
    if(!StartsWith(path, "/"))
      path = "/" + path;
    if(!StartsWith(path, root))
      path = root + path;
    return path;
  }

  static const std::vector<std::string> kVertexShaderExtensions = {
    ".vs",
    ".vsh",
  };
  static const std::vector<std::string> kFragmentShaderExtensions = {
    ".fs",
    ".fsh",
  };

  static inline std::optional<std::string>
  FindShaderPath(const std::string& path, const std::vector<std::string>& extensions) {
    for(const auto& ext : extensions) {
      const auto& new_path = path + ext;
      if(FileExists(new_path))
        return std::optional<std::string>{ new_path };
    }
    return std::nullopt;
  }

  static inline std::optional<std::string>
  FindVertexShaderPath(const std::string& path) {
    return FindShaderPath(path, kVertexShaderExtensions);
  }

  static inline std::optional<std::string>
  FindFragmentShaderPath(const std::string& path) {
    return FindShaderPath(path, kFragmentShaderExtensions);
  }

  bool ShaderResolver::Resolve(const uri::Uri& target) {
    DLOG(INFO) << "resolving " << target << "....";
    const auto path = NormalizePath(root_, target);
    if(HasExtension(path)) {
      std::string ext;
      DLOG_IF(ERROR, !GetExtension(path, ext)) << "failed to get extension for: " << path;
      results_.push_back(path); // probably should assert that ext is .json
      return true;
    }

    if(IsDirectory(path)) {
      if(FileExists(path + "/shader.json")) {
        results_.push_back(path + "/shader.json");
        return true;
      }
    }

    if(FileExists(path + ".json")) {
      results_.push_back(path + ".json");
      return true;
    }

    const auto vert_file = FindVertexShaderPath(path);
    if(!vert_file) {
      DLOG(ERROR) << "cannot find vertex shader file for: " << path;
      return false;
    }
    results_.push_back(vert_file.value());
    
    const auto frag_file = FindFragmentShaderPath(path);
    if(!frag_file) {
      DLOG(ERROR) << "cannot find fragment shader file for: " << path;
      return false;
    }
    results_.push_back(frag_file.value());
    return true;
  }
}