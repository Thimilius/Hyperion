//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <yaml-cpp/yaml.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/core/math/matrix4x4.hpp"

//-------------------- Definition Namespace --------------------
namespace YAML {

  template<> struct convert<Hyperion::Color> {
    static Node encode(const Hyperion::Color &color);
    static bool8 decode(const Node &node, Hyperion::Color &color);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Color &color);
  
  template<> struct convert<Hyperion::Vector2> {
    static Node encode(const Hyperion::Vector2 &vector);
    static bool8 decode(const Node &node, Hyperion::Vector2 &vector);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector2 &vector);

  template<> struct convert<Hyperion::Vector3> {
    static Node encode(const Hyperion::Vector3 &vector);
    static bool8 decode(const Node &node, Hyperion::Vector3 &vector);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector3 &vector);

  template<> struct convert<Hyperion::Vector4> {
    static Node encode(const Hyperion::Vector4 &vector);
    static bool8 decode(const Node &node, Hyperion::Vector4 &vector);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector4 &vector);

  template<> struct convert<Hyperion::Quaternion> {
    static Node encode(const Hyperion::Quaternion &quaternion);
    static bool8 decode(const Node &node, Hyperion::Quaternion &quaternion);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Quaternion &quaternion);

  template<>
  struct convert<Hyperion::Matrix4x4> {
    static Node encode(const Hyperion::Matrix4x4 &matrix);
    static bool8 decode(const Node &node, Hyperion::Matrix4x4 &matrix);
  };
  Emitter &operator <<(Emitter &emitter, const Hyperion::Matrix4x4 &matrix);
  
}
    