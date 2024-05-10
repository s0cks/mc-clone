#ifndef MCC_MOCK_SHADER_H
#define MCC_MOCK_SHADER_H

#include <sstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/shader/shader.h"

namespace mcc::shader {
  class MockShader : public Shader {
  protected:
    ShaderType type_;
  public:
    explicit MockShader(const ShaderId id, const ShaderType type):
      Shader(id),
      type_(type) {
      ON_CALL(*this, AsVertexShader())
        .WillByDefault([type,this]() {
          return type == kVertexShader ? (VertexShader*)this : nullptr;
        });
      ON_CALL(*this, AsFragmentShader())
        .WillByDefault([type,this]() {
          return type == kFragmentShader ? (FragmentShader*)this : nullptr;
        });
      ON_CALL(*this, AsGeometryShader())
        .WillByDefault([type,this]() {
          return type == kGeometryShader ? (GeometryShader*)this : nullptr;
        });
      ON_CALL(*this, AsTessEvalShader())
        .WillByDefault([type,this]() {
          return type == kTessEvalShader ? (TessEvalShader*)this : nullptr;
        });
      ON_CALL(*this, AsTessControlShader())
        .WillByDefault([type,this]() {
          return type == kTessControlShader ? (TessControlShader*)this : nullptr;
        });
    }
    ~MockShader() override = default;

    ShaderType GetType() const override {
      return type_;
    }

    std::string ToString() const override {
      std::stringstream ss;
      ss << "MockShader(";
      ss << "id=" << GetId() << ", ";
      ss << "type=" << GetType();
      ss << ")";
      return ss.str();
    }

    MOCK_METHOD0(AsVertexShader, VertexShader*());
    MOCK_METHOD0(AsFragmentShader, FragmentShader*());
    MOCK_METHOD0(AsGeometryShader, GeometryShader*());
    MOCK_METHOD0(AsTessEvalShader, TessEvalShader*());
    MOCK_METHOD0(AsTessControlShader, TessControlShader*());
  };
}

#endif //MCC_MOCK_SHADER_H