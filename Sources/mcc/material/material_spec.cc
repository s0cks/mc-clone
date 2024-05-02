#include "mcc/material/material_spec.h"

namespace mcc::material {
  class MaterialSpecImpl : public MaterialSpec {
    friend class MaterialSpecBuilder;
  protected:
    std::string name_;
    MaterialComponentSet components_;

    MaterialSpecImpl(const std::string& name,
                     const MaterialComponentSet& components):
      MaterialSpec(),
      name_(name),
      components_(components) {
    }
  public:
    ~MaterialSpecImpl() override = default;

    std::string GetName() const override {
      return name_;
    }

    const MaterialComponentSet& GetComponents() const override {
      return components_;
    }
  };

  MaterialSpec* MaterialSpecBuilder::Build() const {
    return new MaterialSpecImpl(name_, components_);
  }
}