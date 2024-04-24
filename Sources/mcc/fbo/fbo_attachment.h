#ifndef MCC_FBO_ATTACHMENT_H
#define MCC_FBO_ATTACHMENT_H

#include <vector>
#include "mcc/fbo/fbo_target.h"

namespace mcc::fbo {
#define FOR_EACH_FBO_ATTACHMENT_TYPE(V)         \
  V(Color,      GL_COLOR_ATTACHMENT0)           \
  V(Depth,      GL_DEPTH_ATTACHMENT)            \
  V(Stencil,    GL_STENCIL_ATTACHMENT)

#define FORWARD_DECLARE(Name, GlValue) class Name##Attachment;
  FOR_EACH_FBO_ATTACHMENT_TYPE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  class Attachment{
  public:
    enum Type : GLenum {
#define DEFINE_ATTACHMENT(Name, GlValue) k##Name = (GlValue),
      FOR_EACH_FBO_ATTACHMENT_TYPE(DEFINE_ATTACHMENT)
#undef DEFINE_ATTACHMENT
    };
  protected:
    Attachment() = default;

    virtual void AttachTo(const FboTarget target) = 0;
  public:
    virtual ~Attachment() = default;
    virtual Type GetType() const = 0;
    virtual std::string ToString() const = 0;
    virtual const char* GetName() const = 0;

#define DEFINE_TYPE_CHECK(Name, GlValue)                                         \
    virtual Name##Attachment* As##Name##Attachment() { return nullptr; }         \
    bool Is##Name##Attachment() { return As##Name##Attachment() != nullptr; }
    FOR_EACH_FBO_ATTACHMENT_TYPE(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

#define DECLARE_FBO_ATTACHMENT(Name)                                            \
  public:                                                                       \
    std::string ToString() const override;                                      \
    Name##Attachment* As##Name##Attachment() override { return this; }          \
    const char* GetName() const override { return #Name; }                      \
    Type GetType() const override { return Attachment::k##Name; }
}

#include "mcc/fbo/fbo_attachment_color.h"
#include "mcc/fbo/fbo_attachment_depth.h"
#include "mcc/fbo/fbo_attachment_stencil.h"

#endif //MCC_FBO_ATTACHMENT_H