#include "mcc/texture/texture_data.h"

namespace mcc::texture {
  void TextureData::operator=(const img::Image* img) {
    MCC_ASSERT(img);
    switch(img->type()) {
      case img::kRGB:
        format_ = internal_format_ = kRGB;
        break;
      case img::kRGBA:
        format_ = internal_format_ = kRGBA;
        break;
    }
    type_ = kDefaultType;
    size_ = img->size();
  }

  uint64_t TextureData::GetNumberOfBytes() const {
    switch(type()) {
      case GL_UNSIGNED_BYTE:
        return width() * height() * sizeof(uint8_t);
      case GL_UNSIGNED_SHORT:
        return width() * height() * sizeof(uint16_t);
      case GL_UNSIGNED_INT:
        return width() * height() * sizeof(uint32_t);
      default:
        return 0;
    }
  }

  bool TextureData::operator==(const TextureData& rhs) const {
    return type_ == rhs.type_
        && internal_format_ == rhs.internal_format_
        && format_ == rhs.format_
        && bytes_ == rhs.bytes_
        && size_ == rhs.size_
        && level_ == rhs.level_
        && border_ == rhs.border_;
  }

  bool TextureData::operator!=(const TextureData& rhs) const {
    return type_ != rhs.type_
        || internal_format_ != rhs.internal_format_
        || format_ != rhs.format_
        || bytes_ != rhs.bytes_
        || size_ != rhs.size_
        || border_ != rhs.border_
        || level_ != rhs.level_;
  }
}