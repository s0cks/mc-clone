#ifndef MCC_TEXTURE_SOURCE_H
#define MCC_TEXTURE_SOURCE_H

#include <string>
#include <ostream>
#include "mcc/texture/image.h"

namespace mcc::texture {
  class TextureSource {
  protected:
    std::string filename_;

    explicit TextureSource(const std::string& filename):
      filename_(filename) {
    }
  public:
    virtual ~TextureSource() = default;
    
    virtual std::string GetFilename() const {
      return filename_;
    }
  };

  class JpegTextureSource : public TextureSource {
  public:
    explicit JpegTextureSource(const std::string& filename):
      TextureSource(filename) {
    }
    ~JpegTextureSource() override = default;

    friend std::ostream& operator<<(std::ostream& stream, const JpegTextureSource& rhs) {
      stream << "JpegTextureSource(";
      stream << "filename=" << rhs.GetFilename();
      stream << ")";
      return stream;
    }
  };

  class PngTextureSource : public TextureSource {
  public:
    explicit PngTextureSource(const std::string& filename):
      TextureSource(filename) {
    }
    ~PngTextureSource() override = default;

    friend std::ostream& operator<<(std::ostream& stream, const PngTextureSource& rhs) {
      stream << "PngTextureSource(";
      stream << "filename=" << rhs.GetFilename();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_TEXTURE_SOURCE_H