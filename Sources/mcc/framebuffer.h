#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/vertex.h"
#include "mcc/pipeline.h"
#include "mcc/renderbuffer.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"

namespace mcc {
  enum FrameBufferTarget {
    kFrameBuffer = GL_FRAMEBUFFER,

    kDefaultFrameBufferTarget = kFrameBuffer,
  };

  enum FrameBufferTextureAttachment : GLenum {
    kColorAttachment0 = GL_COLOR_ATTACHMENT0,
  };

  enum FrameBufferDepthAttachment : GLenum {
    kDepthAttachment = GL_DEPTH_ATTACHMENT,
    kDepthStencilAttachment = GL_DEPTH_STENCIL_ATTACHMENT,
  };

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
    explicit FrameBufferObject(const BufferObjectId id):
      id_(id) {
    }
    FrameBufferObject(const bool generate = true,
                      const bool bind = true,
                      const bool unbind = true):
      id_(kInvalidBufferObject) {
      Initialize(generate, bind, unbind);
    }
    FrameBufferObject(const FrameBufferObject& rhs):
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

#define DEFINE_ATTACH_DEPTH_BUFFER(Name, _)                                                           \
    void Attach(const Name##Buffer& value) {                                                          \
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, value.target(), value.id());     \
      CHECK_GL(FATAL);                                                                                \
    }
    FOR_EACH_DEPTH_BUFFER_FORMAT(DEFINE_ATTACH_DEPTH_BUFFER)
#undef DEFINE_ATTACH_DEPTH_BUFFER

    template<const google::LogSeverity Severity>
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

  class FrameBuffer {
  private:
    FrameBufferObject fbo_;
    d2::Mesh* mesh_;
    TextureRef cbuff_;
    DepthBuffer dbuff_;
    Dimension size_;

    FrameBuffer(const Dimension& size);
  public:
    FrameBuffer() = delete;
    FrameBuffer(const FrameBuffer& rhs) = delete;
    virtual ~FrameBuffer() = default;

    Dimension size() const {
      return size_;
    }

    FrameBufferObject fbo() const {
      return fbo_;
    }

    TextureRef tex() const {
      return cbuff_;
    }

    d2::Mesh* mesh() const {
      return mesh_;
    }

    void Bind() const {
      return fbo_.Bind();
    }

    void Unbind() {
      return fbo_.Unbind();
    }

    friend std::ostream& operator<<(std::ostream& stream, const FrameBuffer& rhs) {
      stream << "FrameBuffer(";
      stream << "size=" << glm::to_string(rhs.size_) << ", ";
      stream << "tex=" << rhs.cbuff_;
      stream << ")";
      return stream;
    }
  private:
    static void OnPreInit();
    static void OnInit();
    static void OnPostInit();
  public:
    static void Init();
    static FrameBuffer* New(const Dimension& size);

    static inline FrameBuffer*
    New(const uint64_t width, const uint64_t height) {
      return New(Dimension(width, height));
    }
  };
}

#endif //MCC_FRAMEBUFFER_H