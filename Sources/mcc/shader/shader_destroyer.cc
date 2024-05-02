#include "mcc/shader/shader_destroyer.h"
#include "mcc/thread_local.h"
#include "mcc/shader/shader.h"

namespace mcc::shader {
  static ThreadLocal<ShaderDestroyer> instance_;

  ShaderDestroyer* ShaderDestroyer::Get() {
    if(instance_)
      return instance_.Get();
    const auto instance = new ShaderDestroyer();
    instance_.Set(instance);
    return instance;
  }

  void ShaderDestroyer::DestroyShader(Shader* shader) {
    
  }

  void ShaderDestroyer::Destroy(Shader* shader) {

  }
}