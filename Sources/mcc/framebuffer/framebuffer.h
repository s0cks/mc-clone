#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/vertex/vertex.h"
#include "mcc/pipeline.h"
#include "mcc/renderbuffer.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"
#include "mcc/framebuffer/framebuffer_object.h"
#include "mcc/framebuffer/framebuffer_attachment.h"

namespace mcc {
  class FrameBuffer {
  private:
    FrameBufferObject fbo_;
    FrameBufferAttachmentList attachments_;
    DepthBuffer dbuff_;
    Dimension size_;
  public:
    FrameBuffer() = delete;
    explicit FrameBuffer(const Dimension& size,
                         const FrameBufferAttachmentList& attachments);
    FrameBuffer(const FrameBuffer& rhs) = delete;
    virtual ~FrameBuffer() = default;

    Dimension size() const {
      return size_;
    }

    FrameBufferObject fbo() const {
      return fbo_;
    }

    FrameBufferAttachmentList::const_iterator attachments_begin() const {
      return attachments_.begin();
    }

    FrameBufferAttachment* GetAttachment(const uint64_t idx) const {
      return attachments_[idx];
    }

    ColorBufferAttachment* GetColorBufferAttachment(const uint64_t idx) const {
      for(const auto& attachment : attachments_) {
        if(attachment->IsColorBufferAttachment() && attachment->AsColorBufferAttachment()->slot() == idx)
          return attachment->AsColorBufferAttachment();
      }
      return nullptr;
    }

    PickingAttachment* GetPickingAttachment(const uint64_t idx) const {
      for(const auto& attachment : attachments_) {
        if(attachment->IsPickingAttachment() && attachment->AsPickingAttachment()->slot() == idx)
          return attachment->AsPickingAttachment();
      }
      return nullptr;
    }

    void Bind() const {
      fbo_.Bind();
      // glViewport(0, 0, size_[0], size_[1]);
      // CHECK_GL(FATAL);
      //TODO Need to reset image buffer
    }

    void Unbind() const {
      return fbo_.Unbind();
    }

    friend std::ostream& operator<<(std::ostream& stream, const FrameBuffer& rhs) {
      stream << "FrameBuffer(";
      stream << "size=" << glm::to_string(rhs.size_) << ", ";
      //TODO: add attachments
      stream << ")";
      return stream;
    }
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
    static d2::Mesh* GetMesh();
    static FrameBuffer* New(const Dimension& size, const FrameBufferAttachmentList& attachments = {});

    static inline FrameBuffer*
    New(const uint64_t width, const uint64_t height, const FrameBufferAttachmentList& attachments = {}) {
      return New(Dimension(width, height));
    }
  };
}

#endif //MCC_FRAMEBUFFER_H