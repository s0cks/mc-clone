#include "mcc/shader/shader_compiler.h"

#include "mcc/flags.h"
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"

#include "mcc/shader/shader_compiler_status.h"

namespace mcc::shader {
  static inline void
  Attach(const ShaderId id, ShaderCode* code) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(code);
    auto data = code->GetData();
    const auto length = code->GetLength();
    glShaderSource(id, 1, (const GLchar**) &data, (const GLint*) &length);
    CHECK_GL(ERROR);
  }

  static inline void
  AttachAndCompile(const ShaderId id, ShaderCode* code) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(code);
    Attach(id, code);
    glCompileShader(id);
    CHECK_GL(ERROR);
  }

  ShaderId ShaderCompiler::CompileShaderCode(ShaderCode* code) {
    if(!code)
      return kInvalidShaderId;

    using namespace units::time;
    using namespace units::data;

    DLOG(INFO) << "compiling ShaderCode (" << byte_t(code->GetLength()) << ").....";
    const auto start_ns = uv_hrtime();
    const auto id = glCreateShader(code->GetType());
    if(IsValidShaderId(id))
      AttachAndCompile(id, code);
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);
    duration_.Append(total_ns);
    compiled_ += 1;
    DLOG(INFO) << "compilation finished (" << nanosecond_t(total_ns) << ").";
    const auto status = ShaderCompilerStatus(id);
#ifdef MCC_DEBUG
    const auto severity = status ? google::INFO : google::ERROR;
    LOG_AT_LEVEL(severity) << "Shader: " << id;
    if(status) {
      LOG_AT_LEVEL(severity) << "Status: Compiled.";
    } else {
      LOG_AT_LEVEL(severity) << "Status: " << status;
    }
    const auto hash = code->GetSHA256();
    LOG_AT_LEVEL(severity) << "Hash: " << hash.ToHexString();
    LOG_AT_LEVEL(severity) << "Stats:";
    LOG_AT_LEVEL(severity) << " - Total Compiled: " << GetCompiled();
    const auto& duration = GetDurationSeries();
    LOG_AT_LEVEL(severity) << " - Duration: " << nanosecond_t(total_ns) << "; (Avg/Min/Max): " << nanosecond_t(duration_.average()) << ", " << nanosecond_t(duration_.min()) << ", " << nanosecond_t(duration_.min());
    LOG_AT_LEVEL(severity) << "Code (" << byte_t(code->GetLength()) << "):" << std::endl << (*code);
#endif //MCC_DEBUG

    if(!status)
      return kInvalidShaderId; //TODO: cleanup shader id
    Publish<ShaderCompiledEvent>(id);
    return id;
  }

  static ThreadLocal<ShaderCompiler> compiler_;

  static inline bool
  HasCompiler() {
    return (bool) compiler_;
  }

  ShaderCompiler* ShaderCompiler::GetCompiler() {
    if(HasCompiler())
      return compiler_.Get();
    const auto compiler = new ShaderCompiler();
    compiler_.Set(compiler);
    return compiler;
  }

  ShaderId ShaderCompiler::Compile(ShaderCode* code) {
    MCC_ASSERT(code);
    const auto compiler = GetCompiler();
    MCC_ASSERT(compiler);
    return compiler->CompileShaderCode(code);
  } 

  rx::observable<ShaderId> ShaderCompiler::CompileAsync(ShaderCode* code) {
    if(!code)
      return rx::observable<>::empty<ShaderId>();
    return rx::observable<>::create<ShaderId>([code](rx::subscriber<ShaderId> s) {
      const auto compiler = GetCompiler();
      if(!compiler)
        return s.on_error(rx::util::make_error_ptr(std::runtime_error("no ShaderCompiler available on thread.")));
      const auto id = compiler->CompileShaderCode(code);
      s.on_next(id);
      s.on_completed();
    });
  }

  static inline ShaderCode*
  GetShaderCode(const uri::Uri& uri) {
    if(uri.HasScheme("file")) {
      return ShaderCode::FromFile(uri);
    } else if(uri.HasScheme("shader")) {
      const auto abs_path = fmt::format("{0:s}/shaders/{1:s}", FLAGS_resources, uri.path);
      return ShaderCode::FromFile(abs_path);
    }
    DLOG(ERROR) << "invalid shader uri: " << uri;
    return nullptr;
  }

  ShaderId ShaderCompiler::Compile(const uri::Uri& uri) {
    MCC_ASSERT(uri.HasScheme());
    const auto code = GetShaderCode(uri);
    if(!code) {
      DLOG(ERROR) << "failed to get shader code from: " << uri;
      return kInvalidShaderId;
    }
    const auto id = Compile(code);
    delete code;
    return id;
  }

  static inline bool
  IsValidShaderUri(const uri::Uri& uri) {
    if(!uri.HasScheme("shader") || !uri.HasScheme("file"))
      return false;
    return uri.HasExtension(); //TODO: probably should check the actual extension
  }

  rx::observable<ShaderId> ShaderCompiler::CompileAsync(const uri::Uri& uri) {
    if(!IsValidShaderUri(uri))
      return rx::observable<>::error<ShaderId>(std::runtime_error(""));
    return rx::observable<>::create<ShaderId>([uri](rx::subscriber<ShaderId> s) {
      const auto compiler = GetCompiler();
      if(!compiler)
        return s.on_error(rx::util::make_error_ptr(std::runtime_error("No ShaderCompiler available.")));
      const auto code = GetShaderCode(uri);
      if(!code)
        return s.on_error(rx::util::make_error_ptr(std::runtime_error(fmt::format("Cannot get ShaderCode from: {0:s}", (const std::string&)uri))));
      s.on_next(compiler->CompileShaderCode(code));
      delete code;
      s.on_completed();
    });
  }
}