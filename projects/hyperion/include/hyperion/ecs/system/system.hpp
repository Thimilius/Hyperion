//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class ISystem {
  public:
    virtual ~ISystem() = default;
  public:
    virtual void Run(World *world) = 0;
  };

}