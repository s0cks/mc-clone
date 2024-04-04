#ifndef MCC_SHADER_SPEC_JSON_H
#define MCC_SHADER_SPEC_JSON_H

#include "mcc/json.h"
#include "mcc/shader/shader_spec.h"

namespace mcc::shader {
  class JsonShaderSpec : public ShaderSpec, public json::JsonSpec {
    static constexpr const auto kProgramNameProperty = "name";
    static constexpr const auto kFragmentShaderProperty = "fragment";
    static constexpr const auto kVertexShaderProperty = "vertex";
    static constexpr const auto kGeometryShaderProperty = "geometry";
    static constexpr const auto kTessControlShaderProperty = "tessControl";
    static constexpr const auto kTessEvalShaderProperty = "tessEval";
  protected:
    explicit JsonShaderSpec(const uri::Uri& uri, json::DocumentPtr doc);
  public:
    ~JsonShaderSpec() override = default;

    std::optional<uri::Uri> GetFragmentShader() const override {
      return GetUriProperty(kFragmentShaderProperty);
    }

    std::optional<uri::Uri> GetVertexShader() const override {
      return GetUriProperty(kVertexShaderProperty);
    }

    std::optional<uri::Uri> GetGeometryShader() const override {
      return GetUriProperty(kGeometryShaderProperty);
    }

    std::optional<uri::Uri> GetTessEvalShader() const override {
      return GetUriProperty(kTessEvalShaderProperty);
    }

    std::optional<uri::Uri> GetTessControlShader() const override {
      return GetUriProperty(kTessControlShaderProperty);
    }
  public:
    static inline rx::observable<JsonShaderSpec>
    New(const uri::Uri& uri) {
      return json::ParseDocument(uri)
        .map([&uri](const json::DocumentPtr& doc) {
          return JsonShaderSpec{ uri, doc };
        });
    }
  };
}

#endif //MCC_SHADER_SPEC_JSON_H