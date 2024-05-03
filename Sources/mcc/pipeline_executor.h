#ifndef MCC_PIPELINE_EXECUTOR_H
#define MCC_PIPELINE_EXECUTOR_H

#include "mcc/common.h"
#include "mcc/pipeline.h"
#include "mcc/pipeline_events.h"

namespace mcc {
  namespace pipeline {
    class Pipeline;
    class PipelineExecutor : public PipelineVisitor {
    protected:
      Pipeline* pipeline_;
      rx::subject<PipelineEvent*> events_;

      bool Visit(Pipeline* pipeline) override;
    public:
      explicit PipelineExecutor(Pipeline* pipeline):
        pipeline_(pipeline),
        events_() {
        MCC_ASSERT(pipeline);
      }
      virtual ~PipelineExecutor() = default;

      Pipeline* GetPipeline() const {
        return pipeline_;
      }

      const char* GetPipelineName() const;

      rx::observable<PipelineEvent*> OnEvent() const {
        return events_.get_observable();
      }

#define DEFINE_ON_EVENT(Name)                                 \
      inline rx::observable<Name##Event*>                     \
      On##Name##Event() const {                               \
        return OnEvent()                                      \
          .filter(Name##Event::Filter)                        \
          .map(Name##Event::Cast);                            \
      }
      FOR_EACH_PIPELINE_EVENT(DEFINE_ON_EVENT)
#undef DEFINE_ON_EVENT

      bool Execute();
    public:
      static inline bool
      ExecutePipeline(Pipeline* pipeline) {
        MCC_ASSERT(pipeline);
        PipelineExecutor executor(pipeline);
        return executor.Execute();
      }
    };
  }
  using pipeline::PipelineExecutor;
}

#endif //MCC_PIPELINE_EXECUTOR_H