#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glog/logging.h>

#include "mcc/mock_subscription.h"
#include "mcc/shader/shader_source_resolver.h"

namespace mcc {
  using namespace shader;

  class ShaderSourceResolverTest : public ::testing::Test {
  protected:
    ShaderSourceResolverTest() = default;
  public:
    ~ShaderSourceResolverTest() override = default;
  };

  static inline ::testing::Matcher<const ShaderSource&>
  IsShaderSource(const ShaderType type, const std::string& filename) {
    return ::testing::AllOf(
      ::testing::Property("type", &ShaderSource::type, ::testing::Eq(type)),
      ::testing::Property("filename", &ShaderSource::filename, ::testing::Eq(filename))
    );
  }

  TEST_F(ShaderSourceResolverTest, Test_Resolve) {
    rx::MockSubscription<ShaderSource> subscription;
    EXPECT_CALL(subscription, OnNext(IsShaderSource(kFragmentShader, "terrain.frag")));
    EXPECT_CALL(subscription, OnNext(IsShaderSource(kVertexShader, "terrain.vert")));
    EXPECT_CALL(subscription, OnError(::testing::_))
      .Times(0);
    EXPECT_CALL(subscription, OnCompleted());

    const auto onnext = [&subscription](const ShaderSource& next) {
      subscription.OnNext(next);
    };
    const auto onerror = [&subscription](const rx::util::error_ptr& err) {
      subscription.OnError(err);
    };
    const auto oncompleted = [&subscription]() {
      subscription.OnCompleted();
    };

    ShaderSourceResolver resolver(uri::Uri("shader://terrain"));
    resolver.Resolve().subscribe(onnext, onerror, oncompleted);
  }
}