#ifndef MCC_IMAGE_DATA_H
#define MCC_IMAGE_DATA_H

#include <memory>
#include <units.h>

#include "mcc/common.h"
#include "mcc/image/image_constants.h"

namespace mcc {
  class ImageData;
  typedef std::shared_ptr<ImageData> ImageDataPtr;

  class ImageData {
    DEFINE_NON_COPYABLE_TYPE(ImageData);
  private:
    uint32_t num_bytes_;
    uint8_t* bytes_;
  public:
    ImageData() = delete;
    explicit ImageData(const uint32_t num_bytes);
    virtual ~ImageData();

    uint32_t num_bytes() const {
      return num_bytes_;
    }

    uint8_t* bytes() const {
      return bytes_;
    }

    friend std::ostream& operator<<(std::ostream& stream, const ImageData& rhs) {
      using namespace units::data;
      stream << "ImageData(";
      stream << "size=" << to_string(byte_t(rhs.num_bytes())) << ", ";
      stream << "data=" << rhs.bytes();
      stream << ")";
      return stream;
    }
  public:
    static inline ImageData*
    New(const uint32_t num_bytes) {
      return new ImageData(num_bytes);
    }
  };
}

#endif //MCC_IMAGE_DATA_H