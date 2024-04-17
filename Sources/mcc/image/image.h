#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include "mcc/uri.h"
#include "mcc/image/image_data.h"

namespace mcc::img {
  class Image {
  protected:
    uri::Uri uri_;
    ImageType type_;
    ImageSize size_;
    ImageDataPtr data_;
  public:
    Image(const uri::Uri& uri, const ImageType type, const ImageSize& size, const ImageDataPtr& data):
      type_(type),
      size_(size),
      data_(data) {
      MCC_ASSERT(data);
      MCC_ASSERT(uri.HasScheme("file") || uri.HasScheme("img"));
    }
    virtual ~Image() = default;

    uri::Uri uri() const {
      return uri_;
    }

    ImageType type() const {
      return type_;
    }

    ImageSize size() const {
      return size_;
    }

    ImageDataPtr data() const {
      return data_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Image& rhs) {
      stream << "Image(";
      stream << "uri=" << rhs.uri() << ", ";
      stream << "type=" << rhs.type() << ", ";
      stream << "size=" << glm::to_string(rhs.size()) << ", ";
      stream << "data=" << (*rhs.data());
      stream << ")";
      return stream;
    }
  public:
    static inline ImagePtr
    New(const uri::Uri& uri, const ImageType type, const ImageSize size, const ImageDataPtr& data) {
      return std::make_shared<Image>(uri, type, size, data);
    }
  };
}

#include "mcc/image/image_png.h"
#include "mcc/image/image_jpeg.h"

namespace mcc::img {
  rx::observable<ImagePtr> GetImage(const uri::Uri& uri);
}

#endif //MCC_IMAGE_H