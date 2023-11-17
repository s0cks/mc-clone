#ifndef MCC_FRAMEBUFFER_H
#define MCC_FRAMEBUFFER_H

#include "mcc/gfx.h"
#include "mcc/pipeline.h"
#include "mcc/shader/shader.h"
#include "mcc/texture/texture.h"

namespace mcc {
  class FrameBufferObject { //TODO: extend BufferObject somehow
  private:
    BufferObjectId id_;
  public:
    explicit FrameBufferObject(const BufferObjectId id):
      id_(id) {
    }
    FrameBufferObject():
      id_(kInvalidBufferObject) {
      glGenFramebuffers(1, &id_);
      CHECK_GL(FATAL);
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
    }

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

  class RenderBufferObject {
  private:
    BufferObjectId id_;
  public:
    RenderBufferObject(const uint64_t width, const uint64_t height):
      id_(kInvalidBufferObject) {
      glGenRenderbuffers(1, &id_);
      CHECK_GL(FATAL);
      glBindRenderbuffer(GL_RENDERBUFFER, id_);
      CHECK_GL(FATAL);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
      CHECK_GL(FATAL);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, id_);
      CHECK_GL(FATAL);
    }
    explicit RenderBufferObject(const BufferObjectId id = kInvalidBufferObject):
      id_(id) {
    }
    RenderBufferObject(const RenderBufferObject& rhs):
      id_(rhs.id_) {
    }
    virtual ~RenderBufferObject() = default;

    BufferObjectId id() const {
      return id_;
    }

    void operator=(const RenderBufferObject& rhs) {
      id_ = rhs.id_;
    }

    void operator=(const BufferObjectId& rhs) {
      id_ = rhs; //TODO: creates a memory leak
    }

    bool operator==(const RenderBufferObject& rhs) const {
      return id_ == rhs.id_;
    }

    bool operator==(const BufferObjectId& rhs) const {
      return id_ == rhs;
    }

    bool operator!=(const RenderBufferObject& rhs) const {
      return id_ != rhs.id_;
    }

    bool operator!=(const BufferObjectId& rhs) const {
      return id_ != rhs;
    }

    friend std::ostream& operator<<(std::ostream& stream, const RenderBufferObject& rhs) {
      stream << "RenderBufferObject(";
      stream << "id=" << rhs.id_;
      stream << ")";
      return stream;
    }
  };

  struct FrameBufferVertex {
    glm::vec2 pos;
    glm::vec2 uv;
  };

  typedef std::vector<FrameBufferVertex> FrameBufferVertexList;
  class FrameBufferVertexBuffer : public VertexBufferTemplate<FrameBufferVertex, kStaticUsage> {
  public:
    explicit FrameBufferVertexBuffer(const BufferObjectId id = kInvalidBufferObject):
      VertexBufferTemplate(id) {  
    }
    explicit FrameBufferVertexBuffer(const FrameBufferVertex* vertices, const uint64_t num_vertices):
      VertexBufferTemplate(vertices, num_vertices) {
      glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FrameBufferVertex), (const GLvoid*) 0);
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(0);
      CHECK_GL(FATAL);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FrameBufferVertex), (const GLvoid*) offsetof(FrameBufferVertex, uv));
      CHECK_GL(FATAL);
      glEnableVertexAttribArray(1);
      CHECK_GL(FATAL);
    }
    explicit FrameBufferVertexBuffer(const VertexList& vertices):
      FrameBufferVertexBuffer(&vertices[0], vertices.size()) {  
    }
    FrameBufferVertexBuffer(const FrameBufferVertexBuffer& rhs):
      VertexBufferTemplate(rhs) {
    }
    ~FrameBufferVertexBuffer() override = default;
    
    void operator=(const FrameBufferVertexBuffer& rhs) {
      VertexBufferTemplate::operator=(rhs);
    }

    void operator=(const BufferObjectId& rhs) {
      BufferObject::operator=(rhs);
    }
  };
  class DepthBuffer : public gfx::Resource {
  private:
    BufferObjectId id_;
  public:
    DepthBuffer(const uint64_t width, const uint64_t height):
      Resource() {
      glGenRenderbuffers(1, &id_);
      CHECK_GL(FATAL);
      glBindRenderbuffer(GL_RENDERBUFFER, id_);
      CHECK_GL(FATAL);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
      CHECK_GL(FATAL);
    }
    ~DepthBuffer() override = default;

    BufferObjectId id() const {
      return id_;
    }

    void Bind() const override {
      glBindRenderbuffer(GL_RENDERBUFFER, id_);
      CHECK_GL(FATAL);
    }

    void Unbind() const override {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      CHECK_GL(FATAL);
    }

    void Delete() override {
      glDeleteRenderbuffers(1, &id_);
      CHECK_GL(FATAL);
    }
  };

  class FrameBuffer {
  private:
    VertexArrayObject vao_;
    FrameBufferObject fbo_;
    FrameBufferVertexBuffer vbo_;
    TextureRef cbuff_;
    ShaderRef shader_;
    uint64_t width_;
    uint64_t height_;

    FrameBuffer(VertexArrayObject vao, ShaderRef shader, uint64_t width, const uint64_t height);
  public:
    FrameBuffer() = delete;
    FrameBuffer(const FrameBuffer& rhs):
      width_(rhs.width_),
      height_(rhs.height_),
      fbo_(rhs.fbo_),
      cbuff_(rhs.cbuff_),
      vbo_(rhs.vbo_) {
    }
    virtual ~FrameBuffer() = default;

    uint64_t width() const {
      return width_;
    }

    uint64_t height() const {
      return height_;
    }

    VertexArrayObject vao() const {
      return vao_;
    }

    FrameBufferObject fbo() const {
      return fbo_;
    }
    
    FrameBufferVertexBuffer vbo() const {
      return vbo_;
    }

    TextureRef tex() const {
      return cbuff_;
    }

    void Bind() {
      fbo_.Bind();
    }

    void Unbind() {
      fbo_.Unbind();
      cbuff_->Unbind();
      vbo_.Unbind();
    }

    void Draw();

    void operator=(const FrameBuffer& rhs) {
      width_ = rhs.width_;
      height_ = rhs.height_;
      fbo_ = rhs.fbo_;
      cbuff_ = rhs.cbuff_;
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const FrameBuffer& rhs) {
      stream << "FrameBuffer(";
      stream << "width=" << rhs.width_ << ", ";
      stream << "height=" << rhs.height_ << ", ";
      stream << "fbo=" << rhs.fbo_ << ", ";
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
    static FrameBuffer* New(const uint64_t width, const uint64_t size);
  };
}

#endif //MCC_FRAMEBUFFER_H