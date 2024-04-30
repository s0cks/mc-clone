#ifndef MCC_GFX_BIND_SCOPE_H
#define MCC_GFX_BIND_SCOPE_H

namespace mcc::gfx {
  class BindScope {
  public:
    virtual ~BindScope() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
  };
}

#endif //MCC_GFX_BIND_SCOPE_H