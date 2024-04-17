#ifndef MCC_BLOOM_H
#define MCC_BLOOM_H

#include "mcc/framebuffer/framebuffer.h"
#include "mcc/framebuffer/framebuffer_pipeline.h"

namespace mcc {
  using fbuff::FrameBuffer;
  class BlurPipeline : public Pipeline {
  private:
    TextureRef src_;
    FrameBuffer* dst_;
    bool horizontal_;
    ShaderRef shader_;
  public:
    BlurPipeline(TextureRef src,
                 FrameBuffer* dst,
                 const bool horizontal,
                 ShaderRef shader):
      Pipeline(),
      src_(src),
      dst_(dst),
      horizontal_(horizontal),
      shader_(shader) {
      AddChild(new ApplyPipeline([&]() {
        src_->Bind(0);
      }));
      AddChild(new ApplyShaderPipeline(shader, [&](const ShaderRef& s) {
        s->SetInt("tex", 0);
        s->SetBool("horizontal", horizontal_);
      }));
    }
    ~BlurPipeline() override = default;

    void Render() override {
      dst_->Bind();
      InvertedDepthTestScope depth_test;
      RenderChildren();
      FrameBuffer::GetMesh()->Draw();
    }
  };

  template<const int NumberOfHorizontalPasses = 5,
           const int NumberOfVerticalPasses = 5,
           const int NumberOfFrameBuffers = 2>
  class BloomPipeline : public Pipeline {
  private:
    FrameBuffer* src_;
    Dimension size_;
    ShaderRef blur_;
    ShaderRef bloom_;
    FrameBuffer* frame_buffers_[NumberOfFrameBuffers];
  public:
    BloomPipeline(FrameBuffer* src, const Dimension& size, ShaderRef blur):
      Pipeline(),
      src_(src),
      size_(size),
      blur_(blur) {
      for(auto idx = 0; idx < NumberOfFrameBuffers; idx++)
        frame_buffers_[idx] = FrameBuffer::New(size);
    }
    ~BloomPipeline() override {
      for(const auto& fb : frame_buffers_)
        delete fb;
    }

    FrameBuffer* GetFrameBuffer(int idx) const {
      return frame_buffers_[idx];
    }

    void Render() override {
      bool first = true;
      bool flipped = true;
      for(auto idx = 0; idx < NumberOfHorizontalPasses + NumberOfVerticalPasses; idx++) {
        auto src = first ? src_->GetColorBufferAttachment(1)->GetTexture() : frame_buffers_[!flipped]->GetColorBufferAttachment(0)->GetTexture();
        auto dst = frame_buffers_[flipped];
        BlurPipeline pipe(src, dst, flipped, GetShader("blur"));
        pipe.Render();
        if(first) 
          first = false;
        flipped = !flipped;
      }
    }
  };
}

#endif //MCC_BLOOM_H