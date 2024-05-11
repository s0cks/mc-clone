#ifndef MCC_IMAGE_DECODER_H
#define MCC_IMAGE_DECODER_H

#include "mcc/rx.h"
#include "mcc/uri.h"

namespace mcc::img {
  class ImageDecoder {
  protected:
    uri::Uri target_;

    explicit ImageDecoder(const uri::Uri& target):
      target_(target) {
      MCC_ASSERT(target.HasScheme("file"));
    }

    inline uri::Uri& target() {
      return target_;
    }
  public:
    virtual ~ImageDecoder() = default;
    virtual rx::observable<Image*> Decode() = 0;
  };
}

#endif //MCC_IMAGE_DECODER_H