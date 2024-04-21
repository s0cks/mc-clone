#ifndef MCC_FRAMEBUFFER_ATTACHMENT_H
#define MCC_FRAMEBUFFER_ATTACHMENT_H

#include <vector>
#include "mcc/texture/texture.h"

namespace mcc::framebuffer {
#define FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(V) \
  V(ColorBuffer)                                \
  V(RenderBuffer)                               \
  V(Picking)

#define FORWARD_DECLARE(Name) class Name##Attachment;
  FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(FORWARD_DECLARE)
#undef FORWARD_DECLARE

  enum FrameBufferAttachmentType {
#define DEFINE_TYPE(Name) k##Name##Attachment,
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE)
#undef DEFINE_TYPE
    kNumberOfFrameBufferAttachmentTypes,
  };

  typedef GLuint FrameBufferAttachmentTarget;

  class FrameBufferAttachment {
  protected:
    bool enabled_;
    FrameBufferAttachmentTarget target_;

    FrameBufferAttachment(const bool enabled, const FrameBufferAttachmentTarget target):
      enabled_(enabled),
      target_(target) {
    }
    explicit FrameBufferAttachment(const FrameBufferAttachmentTarget target):
      FrameBufferAttachment(true, target) {
    }
  public:
    virtual ~FrameBufferAttachment() = default;
    virtual const char* name() const = 0;
    virtual FrameBufferAttachmentType type() const = 0;

    virtual bool IsEnabled() const {
      return enabled_;
    }

    FrameBufferAttachmentTarget target() const {
      return target_;
    }

#define DEFINE_TYPE_CAST(Name) \
    virtual Name##Attachment* As##Name##Attachment() { return nullptr; }
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE_CAST)
#undef DEFINE_TYPE_CAST

#define DEFINE_TYPE_CHECK(Name) \
    inline bool Is##Name##Attachment() const { return type() == k##Name##Attachment; }
    FOR_EACH_FRAMEBUFFER_ATTACHMENT_TYPE(DEFINE_TYPE_CHECK)
#undef DEFINE_TYPE_CHECK
  };

  typedef std::vector<FrameBufferAttachment*> FrameBufferAttachmentList;

#define DEFINE_FRAMEBUFFER_ATTACHMENT(Name)                                                                     \
  public:                                                                                                       \
    const char* name() const override { return #Name; }                                                         \
    FrameBufferAttachmentType type() const override { return FrameBufferAttachmentType::k##Name##Attachment; }  \
    Name##Attachment* As##Name##Attachment() override { return this; }
}

#include "mcc/framebuffer/framebuffer_attachment_color.h"
#include "mcc/framebuffer/framebuffer_attachment_depth.h"
#include "mcc/framebuffer/framebuffer_attachment_stencil.h"

#endif //MCC_FRAMEBUFFER_ATTACHMENT_H