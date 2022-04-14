//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class Main final {
  public:
    static uint32 Run();
  private:
    Main() = delete;
    ~Main() = delete;
  };

}
