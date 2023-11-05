#ifndef MCC_TEST_CACHE_H
#define MCC_TEST_CACHE_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/cache.h"

namespace mcc {
  class CacheTest : public ::testing::Test {
  protected:
    CacheTest() = default;
  public:
    ~CacheTest() override = default;
  };

  typedef ::testing::MockFunction<uint64_t(const std::string& key)> MockCacheLoader;
}

#endif //MCC_TEST_CACHE_H