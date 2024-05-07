#include "mcc/mock_stdio.h"
#include "mcc/thread_local.h"

namespace mcc {
  static ThreadLocal<MockStdio> mock_;

  MockStdio::MockStdio() {
    mock_.Set(this);
    ON_CALL(*this, fopen(::testing::_, ::testing::_))
      .WillByDefault([](const char* file, const char* mode) {
        return nullptr;
      });
  }

  MockStdio::~MockStdio() {
    mock_.Set(nullptr);
  }
}

  static inline mcc::MockStdio*
  GetMock() {
    const auto mock = mcc::mock_.Get();
    assert(mock != nullptr);
    return mock;
  }

#define __ return GetMock()->

FILE* fopen(const char* file, const char* mode) {
  __ fopen(file, mode);
}