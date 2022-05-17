//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/reflection.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class INonCopyable {
    HYP_REFLECT()
  public:
    INonCopyable() = default;
    INonCopyable(const INonCopyable &other) = delete;
    virtual ~INonCopyable() = default;
  public:
    INonCopyable &operator=(const INonCopyable &other) = delete;
  };

}
