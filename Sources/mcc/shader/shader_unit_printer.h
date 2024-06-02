#ifndef MCC_SHADER_UNIT_PRINTER_H
#define MCC_SHADER_UNIT_PRINTER_H

#include "mcc/shader/shader_unit.h"

namespace mcc::shader {
  class ShaderUnitPrinter : public ShaderCodeVisitor {
  protected:
    google::LogSeverity severity_;

    inline google::LogSeverity GetSeverity() const {
      return severity_;
    }
  public:
    explicit ShaderUnitPrinter(const google::LogSeverity severity):
      ShaderCodeVisitor(),
      severity_(severity) {
    }
    ~ShaderUnitPrinter() override = default;
    
    bool Visit(ShaderCode* code) override {
      LOG_AT_LEVEL(GetSeverity()) << " - " << code->ToString();
      return true;
    }

    bool Print(ShaderUnit* unit) {
      MCC_ASSERT(unit);
      LOG_AT_LEVEL(GetSeverity()) << "Shader Unit:";
      const auto& meta = unit->GetMeta();
      LOG_AT_LEVEL(GetSeverity()) << "Name: " << meta.GetName();
      LOG_AT_LEVEL(GetSeverity()) << "Tags: " << meta.GetTags();
      return unit->Accept(this);
    }
  public:
    template<const google::LogSeverity Severity = google::INFO>
    static inline void
    Print(ShaderUnit* unit) {
      MCC_ASSERT(unit);
      ShaderUnitPrinter printer(Severity);
      return printer.Print(unit);
    }
  };
}

#endif //MCC_SHADER_UNIT_PRINTER_H