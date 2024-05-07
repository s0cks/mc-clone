#include <set>
#include <gtest/gtest.h>
#include "mcc/uri.h"
#include "mcc/mock_stdio.h"
#include "mcc/uri_parser.h"

namespace mcc::uri {
  using namespace ::testing;
  class UriTest : public Test {
  protected:
    UriTest() = default;
  public:
    ~UriTest() override = default;
  };

  TEST_F(UriTest, Test_Parse_ExplicitProtocol) {
    Uri uri("test://hello-world");
    ASSERT_EQ(uri.scheme, "test");
    ASSERT_EQ(uri.path, "hello-world");
  }

  TEST_F(UriTest, Test_HasScheme0) {
    Uri uri("test://example");
    ASSERT_TRUE(uri.HasScheme());
  }

  TEST_F(UriTest, Test_HasScheme1) {
    Uri uri("test://example");
    ASSERT_TRUE(uri.HasScheme("test"));
  }

  TEST_F(UriTest, Test_HasScheme2) {
    Uri uri("test://example");
    ASSERT_TRUE(uri.HasScheme("test", "another"));
  }

  TEST_F(UriTest, Test_HasScheme3) {
    static const std::set<std::string> kSchemes = {
      "test",
      "another",
      "scheme"
    };
    Uri uri("test://example");
    ASSERT_TRUE(uri.HasScheme(kSchemes));
  }

  TEST_F(UriTest, Test_HasExtension0) {
    Uri uri("test:example.png");
    ASSERT_TRUE(uri.HasExtension());
  }

  TEST_F(UriTest, Test_HasExtension1) {
    Uri uri("test:example.png");
    ASSERT_TRUE(uri.HasExtension("png"));
  }

  TEST_F(UriTest, Test_HasExtension2) {
    Uri uri("test:example.png");
    ASSERT_TRUE(uri.HasExtension("png", "another"));
  }

  TEST_F(UriTest, Test_HasExtension3) {
    static const std::set<std::string> kExtensions = {
      "png",
      "another",
      "test"
    };
    Uri uri("test:example.png");
    ASSERT_TRUE(uri.HasExtension(kExtensions));
  }

  static inline AssertionResult
  IsFile(const FILE* file, const short desc) {
    if(!file)
      return AssertionFailure() << "expected file to not be null.";
    if(file->_file != desc)
      return AssertionFailure() << "expected file->_file to be: " << desc << ", actual: " << file->_file;
    return AssertionSuccess();
  }

  TEST_F(UriTest, Test_OpenFile) {
    static constexpr const auto kPath = "/example.png";
    static constexpr const auto kOpenMode = "rb";
    static constexpr const short kFileDesc = 110;
    static constexpr const FILE kFile = {
      ._file = kFileDesc,
    };

    MockStdio stdio;
    EXPECT_CALL(stdio, fopen(StrEq(kPath), StrEq(kOpenMode)))
      .WillOnce([](const char* file, const char* mode) {
        return (FILE*) &kFile;
      });

    Uri uri(fmt::format("file://{0:s}", kPath));
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, kPath);
    const auto file = uri.OpenFile(kOpenMode);
    ASSERT_NE(file, nullptr);
    ASSERT_TRUE(IsFile(file, kFileDesc));
  }

  TEST_F(UriTest, Test_OpenFileForReading) {
    static constexpr const auto kPath = "/example.png";
    static constexpr const short kFileDesc = 120;
    static constexpr const FILE kFile = {
      ._file = kFileDesc,
    };

    MockStdio stdio;
    EXPECT_CALL(stdio, fopen(StrEq(kPath), StrEq("rb")))
      .WillOnce([](const char* file, const char* mode) {
        return (FILE*) &kFile;
      });

    Uri uri(fmt::format("file://{0:s}", kPath));
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, kPath);
    const auto file = uri.OpenFileForReading();
    ASSERT_NE(file, nullptr);
    ASSERT_TRUE(IsFile(file, kFileDesc));
  }

  TEST_F(UriTest, Test_OpenFileForWriting) {
    static constexpr const auto kPath = "/example.png";
    static constexpr const short kFileDesc = 130;
    static constexpr const FILE kFile = {
      ._file = kFileDesc,
    };

    MockStdio stdio;
    EXPECT_CALL(stdio, fopen(StrEq(kPath), StrEq("wb")))
      .WillOnce([](const char* file, const char* mode) {
        return (FILE*) &kFile;
      });

    Uri uri(fmt::format("file://{0:s}", kPath));
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, kPath);
    const auto file = uri.OpenFileForWriting();
    ASSERT_NE(file, nullptr);
    ASSERT_TRUE(IsFile(file, kFileDesc));
  }

  TEST_F(UriTest, Test_GetQuery_Fails_NoKey) {
    Uri uri("file://example.png");
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, "example.png");
    ASSERT_FALSE(uri.HasQuery());
    const auto q1 = uri.GetQuery("test");
    ASSERT_FALSE(q1);
  }

  TEST_F(UriTest, Test_Equality_WillFail_SchemeNotEqual) {
    Uri uri1("file://example.png");
    Uri uri2("test://example.png");
    ASSERT_NE(uri1, uri2);
  }

  TEST_F(UriTest, Test_Equality_WillFail_PathNotEqual) {
    Uri uri1("file://example.png");
    Uri uri2("file://example.jpeg");
    ASSERT_NE(uri1, uri2);
  }

  TEST_F(UriTest, Test_Equality_WillPass) {
    Uri uri1("file://example.png");
    Uri uri2("file://example.png");
    ASSERT_EQ(uri1, uri2);
  }

  TEST_F(UriTest, Test_StringCast) {
    static constexpr const auto kRawUri = "file://example.png";
    const auto uri = Uri(kRawUri);
    const auto str = ((const std::string&) uri);
    ASSERT_EQ(str, kRawUri);
  }

  TEST_F(UriTest, Test_GetParentPath) {
    Uri uri("file:///root/example.png");
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, "/root/example.png");
    ASSERT_EQ(uri.GetParentPath(), "/root");
  }

  TEST_F(UriTest, Test_GetChildPath) {
    Uri uri("file:///root");
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, "/root");
    ASSERT_EQ(uri.GetChildPath("test.png"), "/root/test.png");
  }

  TEST_F(UriTest, Test_GetSiblingPath) {
    Uri uri("file:///root");
    ASSERT_EQ(uri.scheme, "file");
    ASSERT_EQ(uri.path, "/root");
    ASSERT_EQ(uri.GetSiblingPath("test.png"), "/test.png");
  }
}