#ifndef MCC_MOCK_SUBSCRIPTION_H
#define MCC_MOCK_SUBSCRIPTION_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glog/logging.h>

#include "mcc/rx.h"

namespace mcc::rx {
  template<typename T>
  class MockSubscription {
  public:
    explicit MockSubscription() {
      ON_CALL((*this), OnNext)
        .WillByDefault([this](const T& next) {
          DLOG(INFO) << "next: " << next;
        });
      ON_CALL((*this), OnError)
        .WillByDefault([this](rx::error_ptr err) {
          DLOG(ERROR) << "error: " << rx::what(err).c_str();
        });
      ON_CALL((*this), OnCompleted)
        .WillByDefault([this]() {
          DLOG(INFO) << "completed.";
        });
    }
    ~MockSubscription() = default;

    MOCK_METHOD1(OnNext, void(const T&));
    MOCK_METHOD1(OnError, void(rx::error_ptr));
    MOCK_METHOD0(OnCompleted, void());

    rx::composite_subscription Subscribe(rx::observable<T> observable) {
      const auto onnext = [this](const T& next) { return OnNext(next); };
      const auto onerror = [this](const rx::error_ptr& err) { return OnError(err); };
      const auto oncompleted = [this]() { return OnCompleted(); };
      return observable.subscribe(onnext, onerror, oncompleted);
    }
  };
}

#endif //MCC_MOCK_SUBSCRIPTION_H