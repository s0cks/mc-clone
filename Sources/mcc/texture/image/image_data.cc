#include "mcc/texture/image/image_data.h"

namespace mcc {
  ImageData::ImageData(const uint32_t num_bytes):
    num_bytes_(0),
    bytes_(nullptr) {
    using namespace units::data;
    if(num_bytes <= 0) {
      DLOG(WARNING) << "not allocating ImageData bytes of size: " << to_string(byte_t(num_bytes));
      return;
    }
    const auto size = num_bytes;
    const auto data = (uint8_t*) malloc(sizeof(uint8_t) * size);
    if(!data) {
      LOG(ERROR) << "failed to allocate " << to_string(byte_t(size)) << " for ImageData.";
      return;
    }
    bytes_ = data;
    num_bytes_ = size;
  }

  ImageData::~ImageData() {
    if(bytes_) {
      free(bytes_);
      using namespace units::data;
      DLOG(INFO) << "deallocated " << to_string(byte_t(num_bytes_)) << " for ImageData.";
    }
    
    bytes_ = nullptr;
    num_bytes_ = 0;
  }
}