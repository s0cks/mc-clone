#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/mock_gl.h"
#include "mcc/mock_subscription.h"
#include "mcc/shader/shader_linker.h"

namespace mcc::shader {
  class ShaderLinkerTest : public ::testing::Test {
  public:
    ShaderLinkerTest() = default;
    ~ShaderLinkerTest() = default;
  };

  TEST_F(ShaderLinkerTest, Test_Link_WillFail_IdIsInvalid) {
    static constexpr const ShaderId kShader1 = 1;
    static constexpr const ShaderId kShader2 = 2;
    static constexpr const ShaderId kShader3 = 3;
    static constexpr const auto kLinkedShaders = {
      kShader2,
      kShader3,
    };

    MockGl gl;
    rx::MockSubscription<ShaderId> s;
    EXPECT_CALL(s, OnNext(::testing::_)).Times(0);
    EXPECT_CALL(s, OnError(::testing::_));
    //TODO: check error

    ShaderLinker linker;
    s.Subscribe(linker.Link(rx::observable<>::iterate(kLinkedShaders)));
  }

  TEST_F(ShaderLinkerTest, Test_Link_WillFail_glLinkProgramError) {
    static const std::string kErrorMessage = "error.";
    static constexpr const ShaderId kShader1 = 1;
    static constexpr const ShaderId kShader2 = 2;
    static constexpr const ShaderId kShader3 = 3;
    static constexpr const auto kLinkedShaders = {
      kShader2,
      kShader3,
    };

    MockGl gl;
    EXPECT_CALL(gl, glAttachShader(::testing::Eq(kShader1), ::testing::Eq(kShader2)));
    EXPECT_CALL(gl, glAttachShader(::testing::Eq(kShader1), ::testing::Eq(kShader3)));
    EXPECT_CALL(gl, glLinkProgram(::testing::Eq(kShader1)));
    ON_CALL(gl, glGetShaderiv(::testing::Eq(kShader1), ::testing::Eq(GL_LINK_STATUS), ::testing::_))
      .WillByDefault([](GLuint shader, GLenum pname, GLint *params) {
        (*params) = 0;
      });
    ON_CALL(gl, glGetShaderInfoLog(::testing::Eq(kShader1), ::testing::Eq(ShaderStatus::kMessageSize), ::testing::_, ::testing::_))
      .WillByDefault([](GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
        (*length) = kErrorMessage.length();
        memset(&infoLog[0], '\0', bufSize);
        memcpy(&infoLog[0], kErrorMessage.data(), kErrorMessage.length());
      });

    rx::MockSubscription<ShaderId> s;
    EXPECT_CALL(s, OnNext(::testing::Eq(kShader1))).Times(0);
    EXPECT_CALL(s, OnError(::testing::_)).Times(1);
    EXPECT_CALL(s, OnCompleted()).Times(0);
    //TODO: check error
    
    ShaderLinker linker;
    s.Subscribe(linker.Link(kShader1, rx::observable<>::iterate(kLinkedShaders)));
  }

  TEST_F(ShaderLinkerTest, Test_Link_WillPass) {
    static constexpr const ShaderId kShader1 = 1;
    static constexpr const ShaderId kShader2 = 2;
    static constexpr const ShaderId kShader3 = 3;
    static constexpr const auto kLinkedShaders = {
      kShader2,
      kShader3,
    };

    MockGl gl;
    EXPECT_CALL(gl, glAttachShader(::testing::Eq(kShader1), ::testing::Eq(kShader2)));
    EXPECT_CALL(gl, glAttachShader(::testing::Eq(kShader1), ::testing::Eq(kShader3)));
    EXPECT_CALL(gl, glLinkProgram(::testing::Eq(kShader1)));
    ON_CALL(gl, glGetShaderiv(::testing::Eq(kShader1), ::testing::Eq(GL_LINK_STATUS), ::testing::_))
      .WillByDefault([](GLuint shader, GLenum pname, GLint *params) {
        (*params) = 1;
      });
    ON_CALL(gl, glGetShaderInfoLog(::testing::Eq(kShader1), ::testing::Eq(ShaderStatus::kMessageSize), ::testing::_, ::testing::_))
      .WillByDefault([](GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
        memset(&infoLog[0], '\0', bufSize);
      });

    rx::MockSubscription<ShaderId> s;
    EXPECT_CALL(s, OnNext(::testing::Eq(kShader1)));
    EXPECT_CALL(s, OnError(::testing::_)).Times(0);
    EXPECT_CALL(s, OnCompleted());
    
    ShaderLinker linker;
    s.Subscribe(linker.Link(kShader1, rx::observable<>::iterate(kLinkedShaders)));
  }
}