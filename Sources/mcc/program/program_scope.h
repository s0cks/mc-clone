#ifndef MCC_PROGRAM_SCOPE_H
#define MCC_PROGRAM_SCOPE_H

#include "mcc/ubo/ubo.h"

namespace mcc::program {
  class Program;
  class ProgramScope {
  protected:
    Program* program_;

    explicit ProgramScope(Program* program):
      program_(program) {
    }
  public:
    virtual ~ProgramScope() = default;

    Program* GetProgram() const {
      return program_;
    }
  };

  class ProgramUboBindScope : public ProgramScope {
  public:
    struct BoundUbo {
      GLint index;
      std::string name;
      Ubo* ubo;

      BoundUbo() = default;
      BoundUbo(const BoundUbo& rhs) = default;
      ~BoundUbo() = default;

      BoundUbo& operator=(const BoundUbo& rhs) = default;

      friend std::ostream& operator<<(std::ostream& stream, const BoundUbo& rhs) {
        stream << "BoundUbo(";
        stream << "index=" << rhs.index << ", ";
        stream << "name=" << rhs.name << ", ";
        stream << "ubo=" << rhs.ubo->ToString();
        stream << ")";
        return stream;
      }

      bool operator==(const BoundUbo& rhs) const {
        return index == rhs.index;
      }

      bool operator!=(const BoundUbo& rhs) const {
        return index != rhs.index;
      }

      bool operator<(const BoundUbo& rhs) const {
        return index < rhs.index;
      }

      bool operator>(const BoundUbo& rhs) const {
        return index > rhs.index;
      }
    };
  protected:
    std::set<BoundUbo> bound_;
    GLint index_;
  public:
    explicit ProgramUboBindScope(Program* program):
      ProgramScope(program),
      bound_(),
      index_(0) {
    }
    ~ProgramUboBindScope() override = default;
    void Bind(const std::string& name, Ubo* ubo);
  };
}

#endif //MCC_PROGRAM_SCOPE_H