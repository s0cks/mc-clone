#include "mcc/framebuffer.h"
#include "mcc/engine/engine.h"
#include "mcc/shader/shader.h"

namespace mcc {
  static const d2::VertexList kFrameBufferVertices = {
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, -1.0f), .uv = glm::vec2(0.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },

    { .pos = glm::vec2(1.0f, 1.0f), .uv = glm::vec2(1.0f, 1.0f) },
    { .pos = glm::vec2(1.0f, -1.0f), .uv = glm::vec2(1.0f, 0.0f) },
    { .pos = glm::vec2(-1.0f, 1.0f), .uv = glm::vec2(0.0f, 1.0f) },
  };
  static d2::Mesh* kFrameBufferMesh = nullptr;

  d2::Mesh* FrameBuffer::GetMesh() {
    if(kFrameBufferMesh != nullptr)
      return kFrameBufferMesh;
    return kFrameBufferMesh = d2::NewMesh(kFrameBufferVertices);
  }

  FrameBuffer::FrameBuffer(const Dimension& size, const FrameBufferAttachmentList& attachments):
    fbo_(true, true, false),
    attachments_(attachments),
    dbuff_(size),
    size_(size) {
    if(attachments_.empty())
      attachments_.push_back(ColorBufferAttachment::NewDefault(0, size));

    const auto num_attachments = attachments_.size();
    unsigned int draw_buffers[num_attachments];
    unsigned int num_draw_bufffers;
    for(auto idx = 0; idx < num_attachments; idx++) {
      const auto& attachment = attachments_[idx];
      fbo_.Attach(attachment);
      if(attachment->IsDrawBuffer()) //TODO: genericize
        draw_buffers[((ColorBufferAttachment*) attachment)->slot()] = ((ColorBufferAttachment*) attachment)->target();
    }
    if(num_draw_bufffers > 0) {
      glDrawBuffers(num_draw_bufffers, draw_buffers);
      CHECK_GL(FATAL);
    }
    fbo_.Attach(dbuff_);
    fbo_.CheckStatus<google::FATAL>();
    fbo_.Unbind();
#ifdef MCC_VMEMPROF
    DLOG(INFO) << "created: " << (*this);
#endif //MCC_VMEMPROF
  }

  void FrameBuffer::OnPreInit() {
  }

  void FrameBuffer::OnInit() {
  }

  void FrameBuffer::OnPostInit() {
  }

  void FrameBuffer::Init() {
    Engine::OnPreInit(&OnPreInit);
    Engine::OnInit(&OnInit);
    Engine::OnPostInit(&OnPostInit);
  }

  FrameBuffer* FrameBuffer::New(const Dimension& size, const FrameBufferAttachmentList& attachments) {
    return new FrameBuffer(size, attachments);
  }
}