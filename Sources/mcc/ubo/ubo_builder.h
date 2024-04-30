#ifndef MCC_UBO_BUILDER_H
#define MCC_UBO_BUILDER_H

#include "mcc/rx.h"
#include "mcc/ubo/ubo_id.h"

namespace mcc::ubo {
  class Ubo;
  class UboBuilderBase {
  protected:
    UboBuilderBase() = default;

    Ubo* InitUbo(const UboId id);
  public:
    virtual ~UboBuilderBase() = default;
    virtual const uint8_t* GetData() const = 0;
    virtual uint64_t GetLength() const = 0;
    virtual uint64_t GetElementSize() const = 0;
    virtual GLenum GetUsage() const = 0;

    uint64_t GetTotalSize() const {
      return GetElementSize() * GetLength();
    }

    virtual rx::observable<Ubo*> Build() const = 0;
  };

  template<typename T>
  class UboBuilder : public UboBuilderBase {
  public:
    static constexpr const auto kElementSize = sizeof(T);
  protected:
    std::vector<T> data_;
    uint64_t length_;
    GLenum usage_;
  public:
    explicit UboBuilder(const uint64_t length = 0, const GLenum usage = GL_DYNAMIC_DRAW):
      UboBuilderBase(),
      data_(),
      length_(length),
      usage_(usage) {
    }
    ~UboBuilder() override = default;

    uint64_t GetElementSize() const override {
      return kElementSize;
    }

    inline bool HasData() const {
      return !data_.empty();
    }

    uint64_t GetLength() const override {
      return HasData()
           ? data_.size()
           : length_;
    }

    const uint8_t* GetData() const override {
      return HasData()
           ? (const uint8_t*) &data_[0]
           : NULL;
    }

    void SetUsage(const GLenum rhs) {
      usage_ = rhs;
    }

    GLenum GetUsage() const override {
      return usage_;
    }

    void Append(const T& value) {
      data_.push_back(value);
    }

    template<typename C>
    void Append(const C& values) {
      data_.insert(std::end(data_), std::begin(values), std::end(values));
    }

    rx::observable<Ubo*> Build() const override {
      MCC_ASSERT(GetLength() > 0);
      MCC_ASSERT(GetElementSize() > 0);
      return GenerateUboId()
        .filter(IsValidUboId)
        .map([this](const UboId id) {
          return InitUbo(id);
        });
    }
  };
}

#endif //MCC_UBO_BUILDER_H