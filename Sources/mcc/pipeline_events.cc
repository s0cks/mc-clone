#include "mcc/pipeline_events.h"
#include <sstream>
#include "mcc/pipeline.h"

namespace mcc::pipeline {
  std::string PipelineStartedEvent::ToString() const {
    std::stringstream ss;
    ss << "PipelineStartedEvent(";
    ss << "pipeline=" << GetPipeline()->GetName();
    ss << ")";
    return ss.str();
  }

  std::string PipelineFinishedEvent::ToString() const {
    std::stringstream ss;
    ss << "PipelineFinishedEvent(";
    ss << "pipeline=" << GetPipeline()->GetName();
    ss << ")";
    return ss.str();
  }
}