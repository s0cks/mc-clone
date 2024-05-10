#ifndef MCC_PROGRAM_LOADER_DIR_H
#define MCC_PROGRAM_LOADER_DIR_H

#include "mcc/program/program_loader.h"

namespace mcc::program {
  class DirProgramLoader : public ProgramLoader {
  protected:
    std::string root_;
    std::string name_;

    static inline std::string
    GetProgramName(const std::string& root) {
      const auto slashpos = root.find_last_of('/');
      if(slashpos == std::string::npos)
        return root;
      return root.substr(slashpos + 1);
    }
  public:
    DirProgramLoader(const std::string& root,
                     const std::string& name):
      ProgramLoader(),
      root_(root),
      name_(name) {
    }
    explicit DirProgramLoader(const std::string& root):
      DirProgramLoader(root, GetProgramName(root)) {
    }
    ~DirProgramLoader() override = default;

    const std::string& GetRoot() const {
      return root_;
    }

    const std::string& GetName() const {
      return name_;
    }

    Program* LoadProgram() const override;
  public:
    static inline Program*
    Load(const std::string& root, const std::string& name) {
      DirProgramLoader loader(root, name);
      return loader.LoadProgram();
    }

    static inline Program*
    Load(const std::string& root) {
      return Load(root, GetProgramName(root));
    }

    static inline rx::observable<Program*>
    LoadAsync(const std::string& root, const std::string& name) {
      return rx::observable<>::create<Program*>([root,name](rx::subscriber<Program*> s) {
        DirProgramLoader loader(root, name);
        const auto program = loader.LoadProgram();
        if(!program) {
          const auto err = "failed to load Program.";
          return s.on_error(rx::util::make_error_ptr(std::runtime_error(err)));
        }
        s.on_next(program);
        s.on_completed();
      });
    }

    static inline rx::observable<Program*>
    LoadAsync(const std::string& root) {
      return LoadAsync(root, GetProgramName(root));
    }
  };
}

#endif //MCC_PROGRAM_LOADER_DIR_H