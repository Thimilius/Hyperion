//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/types/render_types_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  using MaterialPropertyIndex = uint64;

  struct MaterialProperty {
    ShaderPropertyId id;
    ShaderPropertyType type;
    ShaderPropertyStorage storage = { };
  };

  using MaterialPropertyCollection = Array<MaterialProperty>;
  using MaterialPropertyIndices = Map<ShaderPropertyId, MaterialPropertyIndex>;

}