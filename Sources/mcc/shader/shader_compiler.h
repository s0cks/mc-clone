#ifndef MCC_SHADER_COMPILER_H
#define MCC_SHADER_COMPILER_H

#include <fmt/format.h>

#include "mcc/rx.h"
#include "mcc/series.h"
#include "mcc/shader/shader.h"
#include "mcc/shader/shader_code.h"

namespace mcc::shader {
  class ShaderCompiler {
    DEFINE_NON_COPYABLE_TYPE(ShaderCompiler);
  public:
    typedef TimeSeries<10> DurationSeries;
  protected:
    DurationSeries duration_;
  public:
    ShaderCompiler() = default;
    virtual ~ShaderCompiler() = default;
    virtual ShaderId CompileShaderCode(ShaderCode* source);

    const DurationSeries& GetDurationSeries() const {
      return duration_;
    }
  private:
    static inline uri::Uri
    FormatBasicUri(const uri::basic_uri& uri) {
      if(!(StartsWith(uri, "shader:") || StartsWith(uri, "file:"))) {
        if(StartsWith(uri, "/"))
          return uri::Uri(fmt::format("file://{0:s}", uri));
        return uri::Uri(fmt::format("shader:{0:s}", uri));
      }
      return uri::Uri(uri);
    }
  public:
    static ShaderCompiler* GetCompiler();
    static ShaderId Compile(ShaderCode* code);
    static ShaderId Compile(const uri::Uri& uri);

    static inline ShaderId
    Compile(const uri::basic_uri& uri) {
      return Compile(FormatBasicUri(uri));
    }

    static rx::observable<ShaderId> CompileAsync(ShaderCode* code);
    static rx::observable<ShaderId> CompileAsync(const uri::Uri& uri);

    static inline rx::observable<ShaderId>
    CompileAsync(const uri::basic_uri& uri) {
      return CompileAsync(FormatBasicUri(uri));
    }
  };
}

#endif //MCC_SHADER_COMPILER_H