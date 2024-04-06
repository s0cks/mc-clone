#ifndef MCC_TEXTURE_FILE_H
#define MCC_TEXTURE_FILE_H

#include "mcc/rx.h"
#include "mcc/uri.h"

namespace mcc::texture {
  class TextureFile {
  protected:
    uri::Uri uri_;
  public:
    explicit TextureFile(const uri::Uri& uri):
      uri_(uri) {
      MCC_ASSERT(uri.HasScheme("file"));
    }
    TextureFile(const TextureFile& rhs) = default;
    virtual ~TextureFile() = default;

    uri::Uri uri() const {
      return uri_;
    }

    TextureFile& operator=(const TextureFile& rhs) = default;

    friend std::ostream& operator<<(std::ostream& stream, const TextureFile& rhs) {
      stream << "TextureFile(";
      stream << "uri=" << rhs.uri();
      stream << ")";
      return stream;
    }
  };
}

#endif //MCC_TEXTURE_FILE_H