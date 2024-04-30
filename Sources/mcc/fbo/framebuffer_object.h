#ifndef MCC_FRAMEBUFFER_OBJECT_H
#define MCC_FRAMEBUFFER_OBJECT_H

#include "mcc/renderbuffer.h"
#include "mcc/texture/texture.h"
#include "mcc/resource/resource.h"
#include "mcc/framebuffer/framebuffer_constants.h"
#include "mcc/framebuffer/framebuffer_attachment.h"

namespace mcc::framebuffer {
  class FrameBufferObject : public res::ResourceTemplate<res::kFrameBufferObjectType> { //TODO: extend BufferObject somehow
  private:
    FrameBufferObjectId id_;

    void Destroy() override {
      
    }
  public:
    explicit constexpr FrameBufferObject(const FrameBufferObjectId id = kInvalidFrameBufferId):
      id_(id) {
    }
    constexpr FrameBufferObject(const FrameBufferObject& rhs):
      id_(rhs.id_) {  
    }
    virtual ~FrameBufferObject() = default;

    void Bind() const {
      glBindFramebuffer(GL_FRAMEBUFFER, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      CHECK_GL(FATAL);
    }

    void Delete() {
      glDeleteFramebuffers(1, &id_);
      CHECK_GL(FATAL);
    }

    void Attach(const FrameBufferTextureAttachment attachment, TextureRef texture, const int level = 0) {
      //TODO:
      // glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->target(), texture->id(), level);
      // CHECK_GL(FATAL);
    }

    void Attach(FrameBufferAttachment* attachment, const int level = 0) {
      switch(attachment->type()) {
        case kColorBufferAttachment:
          return Attach((ColorBufferAttachment*) attachment, level);
        default:
          DLOG(ERROR) << "unknown FrameBufferAttachment @" << attachment->name();
      }
    }

    void Attach(ColorBufferAttachment* attachment, const int level = 0) {
      return Attach(attachment->slot(), attachment->GetTexture(), level);
    }

    void Attach(const uint64_t slot, TextureRef texture, const int level = 0) {
      //TODO:
      // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, texture->target(), texture->id(), level);
      // CHECK_GL(FATAL);
    }

#define DEFINE_ATTACH_DEPTH_BUFFER(Name, _)                                                           \
    void Attach(const Name##Buffer& value) {                                                          \
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, value.target(), value.id());     \
      CHECK_GL(FATAL);                                                                                \
    }
    FOR_EACH_DEPTH_BUFFER_FORMAT(DEFINE_ATTACH_DEPTH_BUFFER)
#undef DEFINE_ATTACH_DEPTH_BUFFER

    template<const google::LogSeverity Severity = google::FATAL>
    void CheckStatus() {
      const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      if(status != GL_FRAMEBUFFER_COMPLETE)
        LOG_AT_LEVEL(Severity) << "framebuffer error: " << status;
    }
  public:
    static inline FrameBufferObject*
    New(const FrameBufferObjectId id = kInvalidFrameBufferId) {
      return new FrameBufferObject(id);
    }
  };
  DEFINE_RESOURCE_SCOPE(FrameBufferObject);
}

#endif //MCC_FRAMEBUFFER_OBJECT_H