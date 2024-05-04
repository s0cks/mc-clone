#ifndef MCC_MATERIAL_SCOPE_H
#define MCC_MATERIAL_SCOPE_H

namespace mcc::material {
  class Material;
  class MaterialScope {
  protected:
    Material* material_;
  public:
    explicit MaterialScope(Material* material):
      material_(material) {
    }
    virtual ~MaterialScope() = default;

    Material* GetMaterial() const {
      return material_;
    }
  };

  class MaterialBindScope : public MaterialScope {
  public:
    explicit MaterialBindScope(Material* material):
      MaterialScope(material) {
      Bind();
    }
    ~MaterialBindScope() override {
      Unbind();
    }
    
    void Bind();
    void Unbind();
  };
}

#endif //MCC_MATERIAL_SCOPE_H