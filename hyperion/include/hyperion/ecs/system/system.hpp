//------------------------ Header Guard ------------------------
#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class EntityManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class ISystem {
  public:
    virtual ~ISystem() = default;
  public:
    virtual void Run(EntityManager *manager) = 0;
  };

}
