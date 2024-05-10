#include "mcc/program/program_loader_dir.h"
#include "mcc/file_resolver.h"

#include "mcc/shader/shader.h"
#include "mcc/program/program_builder.h"

namespace mcc::program {
  Program* DirProgramLoader::LoadProgram() const {
    const auto extensions = shader::GetValidFileExtensions();
    
    ProgramBuilder builder;
    DLOG(INFO) << "listing " << name_ << " sources in " << root_ << ": ";
    fs::ListFilesInDirectory(root_)
      .filter(fs::FilterByExtension(extensions))
      .filter([this](const fs::directory_entry& entry) {
        const auto path = fs::ToPath(entry);
        const auto name = path.substr(root_.length() + 1);
        return StartsWith(name, name_);
      })
      .as_blocking()
      .subscribe([&builder](const fs::directory_entry& entry) {
        const auto uri = uri::Uri(fmt::format("file://{0:s}", fs::ToPath(entry)));
        DLOG(INFO) << " - " << uri;
        builder.Attach(uri);
      });
    return builder.Build();
  }
}