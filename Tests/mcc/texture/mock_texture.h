#ifndef MCC_MOCK_TEXTURE_H
#define MCC_MOCK_TEXTURE_H

#include <gmock/gmock.h>

#include "mcc/texture/texture.h"

namespace mcc::texture {
  class MockTexture : public Texture {
  public:
    explicit MockTexture(const TextureId id = kInvalidTextureId):
      Texture(id) {
    }
    ~MockTexture() override = default;

    MOCK_METHOD(void, Bind, (), (const, override));
    MOCK_METHOD(void, Bind, (const uint32_t), (const, override));
    MOCK_METHOD(void, Unbind, (), (const, override));
  private:
    class MockTextureWrapper : public Texture {
    protected:
      MockTexture* delegate_;

      inline MockTexture* Delegate() const {
        return delegate_;
      }
    public:
      explicit MockTextureWrapper(MockTexture* delegate):
        Texture(delegate->id()),
        delegate_(delegate) {
      }
      ~MockTextureWrapper() = default;

      void Bind() const override {
        return Delegate()->Bind();
      }

      void Bind(const uint32_t slot) const override {
        return Delegate()->Bind(slot);
      }

      void Unbind() const override {
        return Delegate()->Unbind();
      }
    };
  public:
    static inline TextureRef
    CreateRef(MockTexture& texture) {
      return TextureRef(new MockTextureWrapper(&texture));
    }
  };
}

#endif //MCC_MOCK_TEXTURE_H