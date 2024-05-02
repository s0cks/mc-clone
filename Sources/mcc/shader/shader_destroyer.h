#ifndef MCC_SHADER_DESTROYER_H
#define MCC_SHADER_DESTROYER_H

#include "mcc/common.h"
#include "mcc/series.h"
#include "mcc/counter.h"

namespace mcc::shader {
  class Shader;
  class ShaderDestroyer {
    DEFINE_NON_COPYABLE_TYPE(ShaderDestroyer);
  public:
    typedef TimeSeries<10> DurationSeries;
    typedef Counter<uint64_t> DestroyedCounter;
  protected:
    DestroyedCounter destroyed_;
    DurationSeries duration_;

    ShaderDestroyer() = default;
  public:
    virtual ~ShaderDestroyer() = default;
    virtual void DestroyShader(Shader* shader);

    const DurationSeries& GetDurationSeries() const {
      return duration_;
    }

    const DestroyedCounter& GetDestroyed() const {
      return destroyed_;
    }
  public:
    static ShaderDestroyer* Get();
    static void Destroy(Shader* shader);
  };
}

#endif //MCC_SHADER_DESTROYER_H