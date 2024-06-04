#include "mcc/shader/shader_compiler.h"

#include "mcc/flags.h"
#include "mcc/uv/utils.h"
#include "mcc/thread_local.h"

#include "mcc/shader/shader_unit_printer.h"
#include "mcc/shader/shader_compile_status.h"

namespace mcc::shader {
  class ShaderSourceAttacher : public ShaderCodeVisitor {
  protected:
    ShaderId target_;
    ShaderUnit* unit_;
    std::vector<GLchar*> sources_;
    std::vector<GLint> lengths_;
    uword num_sources_;
  public:
    ShaderSourceAttacher(const ShaderId target,
                         ShaderUnit* unit):
      ShaderCodeVisitor(),
      target_(target),
      unit_(unit),
      sources_(),
      lengths_(),
      num_sources_(0) {
      MCC_ASSERT(unit);
      const auto num_sources = unit->GetSize();
      sources_.reserve(num_sources);
      lengths_.reserve(num_sources);
    }
    ~ShaderSourceAttacher() override = default;

    ShaderId GetTarget() const {
      return target_;
    }

    ShaderUnit* GetUnit() const {
      return unit_;
    }

    void Append(ShaderCode* code) {
      MCC_ASSERT(code);
      sources_.push_back((GLchar*) code->GetData());
      lengths_.push_back((GLint) code->GetLength());
      num_sources_++;
    }

    bool Visit(ShaderCode* code) override {
      DLOG(INFO) << "attaching: " << code->ToString();
      Append(code);
      return true;
    }

    bool AttachSources() {
      if(!GetUnit()->Accept(this)) {
        DLOG(ERROR) << "failed to visit sources of: " << GetUnit()->ToString();
        return false;
      }

      DLOG(INFO) << "sources:";
      for(auto idx = 0; idx < num_sources_; idx++)
        DLOG(INFO) << " -\n" << std::string(sources_[idx], lengths_[idx]);

      glShaderSource(GetTarget(), num_sources_, &sources_[0], &lengths_[0]);
      CHECK_GL(ERROR);
      return true;
    }
  };

  static inline void
  Compile(const ShaderId id) {
    glCompileShader(id);
    CHECK_GL(ERROR);
  }

  static inline void
  AttachAndCompile(const ShaderId id, ShaderUnit* unit) {
    MCC_ASSERT(IsValidShaderId(id));
    MCC_ASSERT(unit);
    ShaderSourceAttacher attacher(id, unit);
    if(!attacher.AttachSources()) {
      DLOG(ERROR) << "failed to attach sources.";
      return;
    }
    
    Compile(id);
  }

  ShaderId ShaderCompiler::CompileShaderUnit(ShaderUnit* unit) {
    if(!unit)
      return kInvalidShaderId;
    using namespace units::time;
#ifdef MCC_DEBUG
    DLOG(INFO) << "compiling:";
    ShaderUnitPrinter::Print(unit);
#else
    LOG(INFO) << "compiling " << unit->ToString() << "....";
#endif //MCC_DEBUG
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