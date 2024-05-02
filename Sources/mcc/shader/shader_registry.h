#ifndef MCC_SHADER_REGISTRY_H
#define MCC_SHADER_REGISTRY_H

#include <set>
#include "mcc/shader/shader_type.h"
#include "mcc/shader/shader_events.h"

namespace mcc::shader {
  class ShaderRegistry {
  public:
    struct Comparator {
      bool operator()(const Shader* lhs, const Shader* rhs) const;
    };

    typedef std::set<const Shader*, Comparator> ShaderSet;
  protected:
    ShaderSet shaders_;
    rx::subscription on_created_;
    rx::subscription on_destroyed_;

    void Register(const Shader* shader);
    void Deregister(const Shader* shader);
  public:
    explicit ShaderRegistry(const rx::observable<ShaderEvent*>& events);
    explicit ShaderRegistry(const rx::subject<ShaderEvent*>& events):
      ShaderRegistry(events.get_observable()) {
    }
    virtual ~ShaderRegistry();

    ShaderSet::const_iterator begin() const {
      return std::begin(shaders_);
    }

    ShaderSet::const_iterator end() const {
      return std::end(shaders_);
    }

    rx::observable<const Shader*> GetAllShaders() const {
      return rx::observable<>::iterate(shaders_);
    }

#define DECLARE_GET_ALL_SHADERS_BY_TYPE(Name, Ext, GlValue)               \
    rx::observable<const Name##Shader*> GetAll##Name##Shaders() const;
    FOR_EACH_SHADER_TYPE(DECLARE_GET_ALL_SHADERS_BY_TYPE)
#undef DECLARE_GET_ALL_SHADERS_BY_TYPE
  };
}

#endif //MCC_SHADER_REGISTRY_H