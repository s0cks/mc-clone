#ifndef MCC_MOCK_VAO_H
#define MCC_MOCK_VAO_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "mcc/vao/vao.h"

namespace mcc::vao {
  class MockVao : public Vao {
  public:
    explicit MockVao(const VaoId id):
      Vao(id) {
    }
    ~MockVao() override = default;
  };
}

#endif //MCC_MOCK_VAO_H