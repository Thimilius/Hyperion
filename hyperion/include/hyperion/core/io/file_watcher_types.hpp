//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class FileWatcherFileStatus {
    None,
    Created,
    Modified,
    Deleted
  };

  using FileWatcherCallbackFunction = std::function<void(FileWatcherFileStatus, const String &, const String &, const String &)>;

}
