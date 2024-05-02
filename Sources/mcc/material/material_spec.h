#ifndef MCC_MATERIAL_SPEC_H
#define MCC_MATERIAL_SPEC_H

#include <string>
#include <vector>
#include "mcc/material/material_component.h"

namespace mcc {
  namespace texture {
    class Texture;
  }
  using texture::Texture;

  namespace material {
    class MaterialSpec {
    protected:
      MaterialSpec() = default;
    public:
      virtual ~MaterialSpec() = default;
      virtual std::string GetName() const = 0;
      virtual const MaterialComponentSet& GetComponents() const = 0;
    };

    class MaterialSpecBase : public MaterialSpec {
    protected:
      std::string name_;
      MaterialComponentSet components_;

      MaterialSpecBase(const std::string& name,
                       const MaterialComponentSet& components):
        MaterialSpec(),
        name_(name),
        components_(components) {
      }
      explicit MaterialSpecBase(const std::string& name):
        MaterialSpecBase(name, {}) {
      }
    public:
      ~MaterialSpecBase() override = default;

      std::string GetName() const override {
        return name_;
      }

      const MaterialComponentSet& GetComponents() const override {
        return components_;
      }
    };

    class MaterialSpecBuilder {
    protected:
      std::string name_;
      MaterialComponentSet components_;
    public:
      MaterialSpecBuilder(const std::string& name,
                          const MaterialComponentSet& components):
        name_(name),
        components_(components) {
      }
      explicit MaterialSpecBuilder(const std::string& name):
        MaterialSpecBuilder(name, {}) {
      }
      virtual ~MaterialSpecBuilder() = default;

      const std::string& GetName() const {
        return name_;
      }

      const MaterialComponentSet& GetComponents() const {
        return components_;
      }

      bool Add(const MaterialComponent& rhs) {
        const auto result = components_.insert(rhs);
        return result.second;
      }

      bool Add(const std::vector<MaterialComponent>& components) {
        MCC_ASSERT(!components.empty());
        components_.insert(std::begin(components), std::end(components));
        return true;
      }

      bool Add(const MaterialComponentSet& rhs) {
        MCC_ASSERT(!rhs.empty());
        components_.insert(std::begin(rhs), std::end(rhs));
        return true;
      }

      bool Remove(const MaterialComponent& rhs) {
        return components_.erase(rhs) == 1;
      }

      MaterialSpec* Build() const;
    };
  }
}

#include "mcc/material/material_spec_json.h"

#endif //MCC_MATERIAL_SPEC_H