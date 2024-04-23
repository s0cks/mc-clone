#include "mcc/fbo/fbo.h"

namespace mcc::fbo {
//   FrameBuffer::FrameBuffer(const Dimension& size, const FrameBufferAttachmentList& attachments):
//     fbo_(kInvalidFrameBufferId),
//     attachments_(attachments),
//     dbuff_(size),
//     size_(size) {
//     if(attachments_.empty())
//       attachments_.push_back(ColorBufferAttachment::NewDefault(0, size));

//     const auto num_attachments = attachments_.size();
//     unsigned int draw_buffers[num_attachments];
//     unsigned int num_draw_bufffers;
//     for(auto idx = 0; idx < num_attachments; idx++) {
//       const auto& attachment = attachments_[idx];
//       if(!attachment->IsEnabled())
//         continue;
//       fbo_.Attach(attachment);
//       if(attachment->IsColorBufferAttachment()) {
//         draw_buffers[num_draw_bufffers++] = attachment->AsColorBufferAttachment()->target();
//       }
//     }
//     if(num_draw_bufffers > 0) {
//       glDrawBuffers(num_draw_bufffers, draw_buffers);
//       CHECK_GL(FATAL);
//     }
//     fbo_.Attach(dbuff_);
//     fbo_.CheckStatus<google::FATAL>();
//     fbo_.Unbind();
// #ifdef MCC_VMEMPROF
//     DLOG(INFO) << "created: " << (*this);
// #endif //MCC_VMEMPROF
}