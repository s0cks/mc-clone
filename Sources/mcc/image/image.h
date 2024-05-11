#ifndef MCC_IMAGE_H
#define MCC_IMAGE_H

#include "mcc/gfx.h"
#include "mcc/uri.h"
#include "mcc/object.h"
#include "mcc/image/image_format.h"

namespace mcc::img {
  const std::unordered_set<std::string>& GetValidExtensions();

  typedef glm::u32vec2 ImageSize;

  class Image : public Object {
    DEFINE_NON_COPYABLE_TYPE(Image);
  private:
    static inline uword
    GetFormatOffset() {
      return sizeof(Image);
    }

    static inline uword
    GetSizeOffset() {
      return GetFormatOffset() + sizeof(uword);
    }

    static inline uword
    GetDataOffset() {
      return GetSizeOffset() + sizeof(uword);
    }
  protected:
    Image(const ImageFormat format,
          const ImageSize& size):
      Object() {
      SetFormat(format);
      SetSize(size);
    }

    inline uword raw_ptr() const {
      return (uword)this;
    }

    inline ImageFormat* format_ptr() const {
      return (ImageFormat*) (raw_ptr() + GetFormatOffset());
    }

    inline void SetFormat(const ImageFormat& rhs) {
      (*format_ptr()) = rhs;
    }

    inline ImageSize* size_ptr() const {
      return (ImageSize*) (raw_ptr() + GetSizeOffset());
    }

    inline void SetSize(const ImageSize& rhs) {
      (*size_ptr()) = rhs;
    }

    inline uint8_t* data_ptr() const {
      return (uint8_t*) (raw_ptr() + GetDataOffset());
    }

    inline void ClearData() {
      NOT_IMPLEMENTED(FATAL); //TODO: implement
    }
  public:
    ~Image() override = default;
    std::string ToString() const override;

    virtual ImageFormat format() const {
      return *format_ptr();
    }

    const ImageSize& size() const {
      return *size_ptr();
    }

    uint8_t* data() const {
      return data_ptr();
    }

    uword GetTotalSize() const;
  public:
    void* operator new(const size_t sz, const uword num_bytes);
    static Image* New(const ImageFormat format, const ImageSize& size, const uword num_bytes);
  };

  bool Filter(const uri::Uri& uri);
  Image* Decode(const uri::Uri& uri);
  rx::observable<Image*> DecodeAsync(const uri::Uri& uri);
}

#include "mcc/image/image_png.h"
#include "mcc/image/image_jpeg.h"

#endif //MCC_IMAGE_H