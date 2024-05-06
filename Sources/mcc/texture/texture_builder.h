#ifndef MCC_TEXTURE_BUILDER_H
#define MCC_TEXTURE_BUILDER_H

#include "mcc/image/image.h"
#include "mcc/texture/texture_id.h"
#include "mcc/texture/texture_wrap.h"
#include "mcc/texture/texture_target.h"
#include "mcc/texture/texture_format.h"
#include "mcc/texture/texture_filter.h"

namespace mcc::texture {
  class TextureBuilderBase {
  protected:
    TextureTarget target_;
    TextureFilter filter_;
    TextureWrap wrap_;

    explicit TextureBuilderBase(const TextureTarget target):
      target_(target),
      filter_(kLinearFilter),
      wrap_(kClampToEdgeWrap) {
    }

    virtual void InitTexture(const TextureId id) const = 0;
  public:
    virtual ~TextureBuilderBase() = default;
    virtual TextureId Build() const;
    virtual rx::observable<TextureId> BuildAsync() const;

    TextureTarget GetTarget() const {
      return target_;
    }

    const TextureFilter& GetFilter() const {
      return filter_;
    }

    void SetFilter(const TextureFilter& filter) {
      filter_ = filter;
    }

    const TextureWrap& GetWrap() const {
      return wrap_;
    }

    void SetWrap(const TextureWrap& wrap) {
      wrap_ = wrap;
    }
  };

  class TextureBuilder : public TextureBuilderBase {
  public:
    struct TextureData {
      TextureFormat internal_format;
      TextureFormat format;
      GLenum type;
      const uint8_t* bytes;
      uint64_t num_bytes;
      glm::vec2 size;

      TextureData() = default;
      TextureData(const TextureData& rhs) = default;
      ~TextureData() = default;

      TextureData& operator=(const TextureData& rhs) = default;
    };
  private:
    static constexpr const auto kDefaultTextureData = TextureData {
      .internal_format = kDefaultTextureFormat,
      .format = kDefaultTextureFormat,
      .type = GL_UNSIGNED_BYTE,
      .bytes = NULL,
      .num_bytes = 0,
      .size = glm::vec2(0),
    };
  protected:
    int level_;
    int border_;
    TextureData data_;

    void InitTexture(const TextureId id) const override;
  public:
    explicit TextureBuilder(const TextureTarget target):
      TextureBuilderBase(target),
      level_(0),
      border_(0),
      data_(kDefaultTextureData) {
    }
    virtual ~TextureBuilder() = default;

    const TextureFormat& GetFormat() const {
      return data_.format;
    }

    virtual void SetFormat(const TextureFormat format) {
      data_.format = format;
    }

    const TextureFormat& GetInternalFormat() const {
      return data_.internal_format;
    }

    virtual void SetInternalFormat(const TextureFormat format) {
      data_.internal_format = format;
    }

    const glm::vec2& GetSize() const {
      return data_.size;
    }

    virtual void SetSize(const TextureSize& size) {
      data_.size = size;
    }

    float GetWidth() const {
      return data_.size[0];
    }

    float GetHeight() const {
      return data_.size[1];
    }

    const int& GetLevel() const {
      return level_;
    }

    virtual void SetLevel(const int level) {
      level_ = level;
    }

    const int& GetBorder() const {
      return border_;
    }

    virtual void SetBorder(const int border) {
      border_ = border;
    }

    const GLenum& GetType() const {
      return data_.type;
    }

    virtual void SetType(const GLenum type) {
      data_.type = type;
    }

    virtual void SetData(const uint8_t* bytes, const uint64_t num_bytes) {
      MCC_ASSERT(bytes);
      MCC_ASSERT(num_bytes > 0);
      data_.bytes = bytes;
      data_.num_bytes = num_bytes;
    }

    inline void SetData(ImageData* image) {
      return SetData(image->bytes(), image->num_bytes());
    }

    virtual const GLvoid* GetData() const {
      return (const GLvoid*) data_.bytes;
    }

    void Attach(img::Image* image);
  };
}

#endif //MCC_TEXTURE_BUILDER_H