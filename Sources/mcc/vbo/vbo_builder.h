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
    Vbo* InitVbo(const VboId id) const {
      MCC_ASSERT(IsValidVboId(id));
      const auto total_size = GetLength() * GetVertexLength();
      Vbo::BindVbo(id);
      BindAndEnableAll<Attributes...>();
      Vbo::PutVboData(GetVertexData(), total_size, GetUsage());
      Vbo::BindDefaultVbo();
      return Vbo::New(id, GetLength(), GetVertexLength(), GetUsage());
    }
  public:
    virtual ~VboBuilderBase() = default;
    virtual uint64_t GetLength() const = 0;
    virtual uint64_t GetVertexLength() const = 0;
    virtual const uint8_t* GetVertexData() const = 0;
    virtual gfx::Usage GetUsage() const = 0;
    virtual rx::observable<Vbo*> Build(const int num = 1) const = 0;
  };

  template<class Vertex,
           class... Attributes>
  class VboBuilderTemplate : public VboBuilderBase {
  private:
    typedef std::vector<Vertex> VertexList;
  protected:
    VertexList data_;
    uint64_t length_;
    gfx::Usage usage_;

    VboBuilderTemplate(const VertexList& data, 
                       const uint64_t length,
                       const gfx::Usage usage):
      VboBuilderBase(),
      data_(data),
      length_(length),
      usage_(usage) {
    }
  public:
    ~VboBuilderTemplate() override = default;

    gfx::Usage GetUsage() const override {
      return usage_;
    }

    uint64_t GetLength() const override {
      return data_.empty()
           ? length_
           : data_.size();
    }

    uint64_t GetVertexLength() const override {
      return sizeof(Vertex);
    }

    const uint8_t* GetVertexData() const override {
      return data_.empty()
           ? NULL
           : (const uint8_t*) &data_[0];
    }

    void Append(const Vertex& vertex) {
      data_.push_back(vertex);
    }

    template<typename C>
    void Append(const C& vertices) {
      data_.insert(std::end(data_), std::begin(vertices), std::end(vertices));
    }

    rx::observable<Vbo*> Build(const int num = 1) const override {
      return GenerateVboId(num)
        .filter(IsValidVboId)
        .map([this](const VboId id) {
          return VboBuilderBase::InitVbo<Attributes...>(id);
        });
    }
  };

  template<class Vertex,
           class... Attributes>
  class VboBuilder : public VboBuilderTemplate<Vertex, Attributes...> {
  public:
    typedef std::vector<Vertex> VertexList;
  public:
    VboBuilder(const VertexList& vertices, const gfx::Usage usage):
      VboBuilderTemplate<Vertex, Attributes...> (vertices, 0, usage) {
    }
    explicit VboBuilder(const uint64_t num_vertices = 0, const gfx::Usage usage = gfx::kDefaultUsage):
      VboBuilderTemplate<Vertex, Attributes...> (VertexList{}, num_vertices, usage) {
    }
    ~VboBuilder() override = default;
  };
}

#endif //MCC_VBO_FACTORY_H