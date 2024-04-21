#include <gtest/gtest.h>
#include <glog/logging.h>

#include "mcc/mcc.h"

int main(int argc, char** argv) {
  ::google::InitGoogleLogging(argv[0]);
  ::google::LogToStderr();

  ::testing::InitGoogleTest(&argc, argv);
  ::google::ParseCommandLineFlags(&argc, &argv, false);
  LOG(INFO) << "Running unit tests for minecraft-clone v" << mcc::GetVersion() << "....";

  return RUN_ALL_TESTS();
}