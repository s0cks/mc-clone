#include "mcc/pipeline.h"
#include "mcc/pipeline_executor.h"

namespace mcc {
  bool Pipeline::Execute() {
    return PipelineExecutor::ExecutePipeline(this);
  }
}