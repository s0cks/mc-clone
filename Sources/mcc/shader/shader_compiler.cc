#include "mcc/shader/shader_compiler.h"

#include "mcc/flags.h"
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"

#include "mcc/shader/shader_compile_status.h"

namespace mcc::shader {
  static inline void
  Attach(const ShaderId id, const ShaderCode* code) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(code);
    auto data = code->GetData();
    const auto length = code->GetLength();
    glShaderSource(id, 1, (const GLchar**) &data, (const GLint*) &length);
    CHECK_GL(ERROR);
  }

  static inline void
  Attach(const ShaderId id, const ShaderUnit* unit) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(unit);
    MCC_ASSERT(!unit->IsEmpty());
    const auto num_sources = unit->GetSize();
    GLint lengths[num_sources];
    GLchar* sources[num_sources];
    for(auto idx = 0; idx < num_sources; idx++) {
      const auto code = unit->GetSource(idx);
      sources[idx] = (GLchar*) code->GetData();
      lengths[idx] = code->GetLength();
    }
    glShaderSource(id, num_sources, sources, lengths);
    CHECK_GL(ERROR);
  }

  static inline void
  Compile(const ShaderId id) {
    glCompileShader(id);
    CHECK_GL(ERROR);
  }

  static inline void
  AttachAndCompile(const ShaderId id, const ShaderCode* code) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(code);
    Attach(id, code);
    Compile(id);
  }

  static inline void
  AttachAndCompile(const ShaderId id, const ShaderUnit* unit) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(unit);
    Attach(id, unit);
    Compile(id);
  }

  ShaderId ShaderCompiler::CompileShaderUnit(ShaderUnit* unit) {
    if(!unit)
      return kInvalidShaderId;
    using namespace units::time;
    DLOG(INFO) << "compiling " << unit->ToString() << "....";
    const auto start_ns = uv_hrtime();
    const auto id = glCreateShader(unit->GetType());
    if(IsValidShaderId(id))
      AttachAndCompile(id, unit);
    const auto stop_ns = uv_hrtime();
    const auto total_ns = (stop_ns - start_ns);
    duration_.Append(total_ns);
    compiled_ += 1;
    DLOG(INFO) << "compilation finished (" << nanosecond_t(total_ns) << ").";
    const auto status = ShaderCompileStatus(id);
#ifdef MCC_DEBUG
    using namespace units::data;
    const auto severity = status ? google::INFO : google::ERROR;
    LOG_AT_LEVEL(severity) << "Shader: " << id;
    LOG_AT_LEVEL(severity) << "Status: " << status;
    LOG_AT_LEVEL(severity) << "Unit: " << unit->ToString();
    LOG_AT_LEVEL(severity) << "Stats:";
    LOG_AT_LEVEL(severity) << " - Total Compiled: " << GetCompiled();
    const auto& duration = GetDurationSeries();
    LOG_AT_LEVEL(severity) << " - Duration: " << nanosecond_t(total_ns) << "; (Avg/Min/Max): " << nanosecond_t(duration_.average()) << ", " << nanosecond_t(duration_.min()) << ", " << nanosecond_t(duration_.min());
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

  ShaderId ShaderCompiler::Compile(ShaderUnit* unit) {
    MCC_ASSERT(unit);
    const auto compiler = GetCompiler();
    MCC_ASSERT(compiler);
    return compiler->CompileShaderUnit(unit);
  }

  rx::observable<ShaderId> ShaderCompiler::CompileAsync(ShaderUnit* unit) {
    if(!unit)
      return rx::observable<>::empty<ShaderId>();

    return rx::observable<>::create<ShaderId>([unit](rx::subscriber<ShaderId> s) {
      const auto compiler = GetCompiler();
      if(!compiler)
        return s.on_error(rx::util::make_error_ptr(std::runtime_error("no ShaderCompiler available on thread.")));
      const auto id = compiler->CompileShaderUnit(unit);
      s.on_next(id);
      s.on_completed();
    });
  }
}