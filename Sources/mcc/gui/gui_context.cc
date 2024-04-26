#include "mcc/gui/gui_context.h"

#include "mcc/thread_local.h"
#include "mcc/vbo/vbo_builder.h"

namespace mcc::gui {
  static ThreadLocal<Vao> vao_;
  static ThreadLocal<Vbo> vbo_;
  static ThreadLocal<Ibo> ibo_;
  static ThreadLocal<Context> ctx_;

  static inline Vao*
  CreateVao() {
    return Vao::New();
  }

  static inline Vao*
  SetVao(Vao* vao) {
    MCC_ASSERT(vao);
    vao_.Set(vao);
    return vao;
  }

  static inline Vao*
  GetVao() {
    return vao_.Get();
  }

  static inline bool
  HasVao() {
    return (bool) vao_;
  }

  static inline Vao*
  GetOrCreateVao() {
    if(HasVao())
      return GetVao();
    return SetVao(CreateVao());
  }

  static inline Vbo*
  CreateVbo(const int32_t num_vertices) {
    NOT_IMPLEMENTED(FATAL);//TODO: implement
    return nullptr;
  }

  static inline Vbo*
  SetVbo(Vbo* vbo) {
    vbo_.Set(vbo);
    return vbo;
  }

  static inline Vbo*
  GetVbo() {
    return vbo_.Get();
  }

  static inline bool
  HasVbo() {
    return (bool) vbo_;
  }

  static inline Vbo*
  GetOrCreateVbo(const int32_t num_vertices) {
    if(HasVbo())
      return GetVbo();
    return SetVbo(CreateVbo(num_vertices));
  }

  static inline Ibo*
  CreateIbo(const int32_t num_indices) {
    return UIntIbo::New(num_indices, ibo::kDefaultUsage);
  }

  static inline Ibo*
  SetIbo(Ibo* ibo) {
    MCC_ASSERT(ibo);
    ibo_.Set(ibo);
    return ibo;
  }

  static inline Ibo*
  GetIbo() {
    return ibo_.Get();
  }

  static inline bool
  HasIbo() {
    return GetIbo() != nullptr;
  }

  static inline Ibo*
  GetOrCreateIbo(const int32_t num_indices) {
    if(HasIbo())
      return GetIbo();
    return SetIbo(CreateIbo(num_indices));
  }

  Context::Context(const int32_t num_vertices,
                   const int32_t num_indices) {
  }

  void Context::SetCurrent(Context* ctx) {
    MCC_ASSERT(ctx);
    MCC_ASSERT(IsOpen());
    ctx_.Set(ctx);
  }

  Context* Context::GetCurrent() {
    DLOG_IF(WARNING, IsOpen()) << "no gui::Context to get.";
    return ctx_.Get();
  }
}