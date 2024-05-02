#include "mcc/shader/shader_registry.h"

#include <functional>
#include "mcc/shader/shader.h"

namespace mcc::shader {
  bool ShaderRegistry::Comparator::operator()(const Shader* lhs, const Shader* rhs) const {
    return lhs->Equals(rhs);
  }

  ShaderRegistry::ShaderRegistry(const rx::observable<ShaderEvent*>& events):
    shaders_(),
    on_created_(),
    on_destroyed_() {
    on_created_ = events
      .filter(ShaderCreatedEvent::Filter)
      .map(ShaderCreatedEvent::Cast)
      .subscribe([this](ShaderCreatedEvent* event) {
        return Register(event->GetShader());
      });
    on_destroyed_ = events
      .filter(ShaderDestroyedEvent::Filter)
      .map(ShaderDestroyedEvent::Cast)
      .subscribe([this](ShaderDestroyedEvent* event) {
        return Deregister(event->GetShader());
      });
  }

  ShaderRegistry::~ShaderRegistry() {
    if(on_created_.is_subscribed())
      on_created_.unsubscribe();
    if(on_destroyed_.is_subscribed())
      on_destroyed_.unsubscribe();
  }

  void ShaderRegistry::Register(const Shader* shader) {
    const auto result = shaders_.insert(shader);
    LOG_IF(WARNING, !result.second) << "failed to register: " << shader->ToString();
  }

  void ShaderRegistry::Deregister(const Shader* shader) {
    const auto result = shaders_.erase(shader);
    LOG_IF(WARNING, result != 1) << "failed to deregister: " << shader->ToString();
  }

  static inline std::function<bool(const Shader*)>
  CreateTypeFilter(const ShaderType type) {
    return [type](const Shader* shader) {
      return shader
          && shader->GetType() == type;
    };
  }

  template<class S>
  static inline std::function<const S*(const Shader*)>
  Cast() {
    return [](const Shader* shader) {
      return reinterpret_cast<const S*>(shader);
    };
  }

#define DEFINE_GET_ALL_SHADERS_BY_TYPE(Name, Ext, GlValue)            \
  rx::observable<const Name##Shader*>                                 \
  ShaderRegistry::GetAll##Name##Shaders() const {                     \
    return GetAllShaders()                                            \
      .filter(CreateTypeFilter(k##Name##Shader))                      \
      .map(Cast<Name##Shader>());                                     \
  }
  FOR_EACH_SHADER_TYPE(DEFINE_GET_ALL_SHADERS_BY_TYPE)
#undef DEFINE_GET_ALL_SHADERS_BY_TYPE
}