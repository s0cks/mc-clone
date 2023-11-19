#ifndef MCC_FRAMEBUFFER_OBJECT_H
#define MCC_FRAMEBUFFER_OBJECT_H

#include "mcc/renderbuffer.h"
#include "mcc/texture/texture.h"
#include "mcc/framebuffer_constants.h"
#include "mcc/framebuffer_attachment.h"

namespace mcc {
  class FrameBufferObject { //TODO: extend BufferObject somehow
  private:
    BufferObjectId id_;

    inline void Initialize(const bool generate = true,
                           const bool bind = true,
                           const bool unbind = true) {
      if(!generate)
        return;
      glGenFramebuffers(1, &id_);
      CHECK_GL(FATAL);
      if(!bind)
        return;
      glBindFramebuffer(GL_FRAMEBUFFER, id_);
      CHECK_GL(FATAL);
      if(!unbind)
        return;
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      CHECK_GL(FATAL);
    }
  public:
    explicit constexpr FrameBufferObject(const BufferObjectId id = kInvalidBufferObject):
      id_(id) {
    }
    FrameBufferObject(const bool generate = true,
                      const bool bind = true,
                      const bool unbind = true):
      id_(kInvalidBufferObject) {
      Initialize(generate, bind, unbind);
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
      glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->target(), texture->id(), level);
      CHECK_GL(FATAL);
    }

    void Attach(FrameBufferAttachment* attachment, const int level = 0) {
      switch(attachment->type()) {
        case kColorBufferAttachment:
          return Attach((ColorBufferAttachment*) attachment, level);
        case kPickingAttachment:
          return Attach(attachment->AsPickingAttachment(), level);
        default:
          DLOG(ERROR) << "unknown FrameBufferAttachment @" << attachment->name();
      }
    }

    void Attach(ColorBufferAttachment* attachment, const int level = 0) {
      return Attach(attachment->slot(), attachment->GetTexture(), level);
    }

    void Attach(PickingAttachment* attachment, const int level = 0) {
      return Attach(attachment->slot(), attachment->GetTexture(), level);
    }

    void Attach(const uint64_t slot, TextureRef texture, const int level = 0) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, texture->target(), texture->id(), level);
      CHECK_GL(FATAL);
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

    void operator=(const FrameBufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const BufferObjectId& rhs) {
      id_ = rhs;
    }

    bool operator==(const FrameBufferObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const BufferObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const FrameBufferObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const BufferObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const FrameBufferObject& rhs) {
      stream << "FrameBufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };
  DEFINE_RESOURCE_SCOPE(FrameBufferObject);

  static constexpr const BufferObjectId kDefaultFrameBufferObjectId = 0;
}

#endif //MCC_FRAMEBUFFER_OBJECT_H