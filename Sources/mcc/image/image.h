#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include "mcc/uri.h"
#include "mcc/image/image_data.h"

namespace mcc::img {
  class Image {
  protected:
    ImageType type_;
    ImageSize size_;
    ImageData* data_;
  public:
    Image(const ImageType type, const ImageSize& size, ImageData* data):
      type_(type),
      size_(size),
      data_(data) {
      MCC_ASSERT(data);
    }
    virtual ~Image() = default;

    ImageType type() const {
      return type_;
    }

    ImageSize size() const {
      return size_;
    }

    ImageData* data() const {
      return data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Image& rhs) {
      stream << "Image(";
      stream << "type=" << rhs.type() << ", ";
      stream << "size=" << glm::to_string(rhs.size()) << ", ";
      stream << "data=" << (*rhs.data());
      stream << ")";
      return stream;
    }
  public:
    static inline Image*
    New(const ImageType type, const ImageSize size, ImageData* data) {
      return new Image(type, size, data);
    }
  };
}

#include "mcc/image/image_png.h"
#include "mcc/image/image_jpeg.h"

namespace mcc::img {
  rx::observable<Image*> GetImage(const uri::Uri& uri);
}

#endif //MCC_IMAGE_H