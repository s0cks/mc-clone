#ifndef MCC_PROGRAM_LOADER_JSON_H
#define MCC_PROGRAM_LOADER_JSON_H

#include "mcc/uri.h"
#include "mcc/flags.h"
#include "mcc/common.h"
#include "mcc/shader/shader.h"
#include "mcc/program/program_loader.h"

namespace mcc::program {
  class JsonProgramLoader : public ProgramLoader {
    static inline std::string
    GetProgramResourceDirectory() {
      return fmt::format("{0:s}/shaders", FLAGS_resources);
    }

    static inline uri::Uri
    NormalizeUri(const uri::Uri& uri) {
      const auto root_dir = GetProgramResourceDirectory();
      if(uri.HasScheme("program")) {
        if(uri.HasExtension("json"))
          return uri::Uri(fmt::format("file://{0:s}/{1:s}", root_dir, uri.path));
        return uri::Uri(fmt::format("file://{0:s}/{1:s}.json", root_dir, uri.path));
      }
      MCC_ASSERT(uri.HasScheme("file"));
      if(uri.HasExtension("json"))
        return uri;
      MCC_ASSERT(!uri.HasExtension());
      auto path = uri.path;
      if(!StartsWith(path, root_dir))
        path = fmt::format("{0:s}/{1:s", root_dir, path[0] == '/' ? path.substr(1) : path);
      return uri::Uri(fmt::format("file://{0:s}", path));
    }
  protected:
    uri::Uri uri_;

    inline const uri::Uri& GetUri() const {
      return uri_;
    }

    Shader* LoadVertexShader() const;
    Shader* LoadFragmentShader() const;
    Shader* LoadGeometryShader() const;
    Shader* LoadTessEvalShader() const;
    Shader* LoadTessControlShader() const;
  public:
    explicit JsonProgramLoader(const uri::Uri& uri):
      ProgramLoader(),
      uri_(NormalizeUri(uri)) {
      MCC_ASSERT(GetUri().IsAbsolutePath());
      MCC_ASSERT(GetUri().HasExtension("json"));
    }
    ~JsonProgramLoader() override = default;
    Program* LoadProgram() const override;
  };
}

#endif //MCC_PROGRAM_LOADER_JSON_H