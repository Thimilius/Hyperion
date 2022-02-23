//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class WorldSerializer final {
  public:
    static String Serialize(World *world);
    static World *Deserialize(const String &data);
  private:
    WorldSerializer() = delete;
    ~WorldSerializer() = delete;
  };

}