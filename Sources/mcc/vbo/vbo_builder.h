#ifndef MCC_VBO_FACTORY_H
#define MCC_VBO_FACTORY_H

#include "mcc/vbo/vbo.h"

namespace mcc::vbo {
  class VboBuilderBase {
  protected:
    VboBuilderBase() = default;

    template<typename A>
    static inline void BindAndEnable() { }

    template<typename A, typename B, typename... Tail>
    static inline void BindAndEnable() {
      B::BindAndEnable();
      BindAndEnable<A, Tail...>();
    }

    template<typename... Attributes>
    static inline void BindAndEnableAll() {
      BindAndEnable<void, Attributes...>();
    }

    template<typename... Attributes>
    Vbo* CreateVbo(const VboId id);
  public:
    virtual ~VboBuilderBase() = default;
    virtual uint64_t GetLength() const = 0;
    virtual uint64_t GetVertexLength() const = 0;
    virtual const uint8_t* GetVertexData() const = 0;
    virtual VboUsage GetUsage() const = 0;
    virtual rx::observable<Vbo*> Build(const VboUsage usage, const int num = 1) const = 0;
  };

  template<class Vertex,
           class... Attributes>
  class VboBuilder : public VboBuilderBase {
  public:
    typedef std::vector<Vertex> VertexList;
  protected:
    VboUsage usage_;
    VertexList data_;
  public:
    explicit VboBuilder(const VboUsage usage):
      usage_(usage) {
    }
    ~VboBuilder() override = default;

    uint64_t GetLength() const override {
      return data_.size();
    }

    uint64_t GetVertexLength() const override {
      return sizeof(Vertex);
    }

    const uint8_t* GetVertexData() const override {
      return (const uint8_t*) &data_[0];
    }

    VboUsage GetUsage() const override {
      return usage_;
    }

    void Append(const Vertex& vertex) {
      data_.push_back(vertex);
    }

    template<typename C>
    void Append(const C& vertices) {
      data_.insert(std::end(data_), std::begin(vertices), std::end(vertices));
    }

    rx::observable<Vbo*> Build(const VboUsage usage, const int num = 1) const override {
      return GenerateVboId(num)
        .filter(IsValidVboId)
        .map([this,usage](VboId id) {
          return CreateVbo(id);
        });
    }
  };
}

#endif //MCC_VBO_FACTORY_H