//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/yaml/yaml_serialization.hpp"

//-------------------- Definition Namespace --------------------
namespace YAML {

  //--------------------------------------------------------------
  Node convert<Hyperion::Color>::encode(const Hyperion::Color &color) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["r"] = color.r;
    node["g"] = color.g;
    node["b"] = color.b;
    node["a"] = color.a;
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Color>::decode(const Node &node, Hyperion::Color &color) {
    if (!node.IsMap() || node.size() != 4) {
      return false;
    }

    auto yaml_r = node["r"];
    if (!yaml_r || !yaml_r.IsScalar()) {
      return false;
    }
    color.r = yaml_r.as<float32>();

    auto yaml_g = node["g"];
    if (!yaml_g || !yaml_g.IsScalar()) {
      return false;
    }
    color.g = yaml_g.as<float32>();

    auto yaml_b = node["b"];
    if (!yaml_b || !yaml_b.IsScalar()) {
      return false;
    }
    color.b = yaml_b.as<float32>();

    auto yaml_a = node["a"];
    if (!yaml_a || !yaml_a.IsScalar()) {
      return false;
    }
    color.a = yaml_a.as<float32>();

    return true;
  }

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Color &color) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "r" << Value << color.r;
    emitter << Key << "g" << Value << color.g;
    emitter << Key << "b" << Value << color.b;
    emitter << Key << "a" << Value << color.a;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  Node convert<Hyperion::Vector2>::encode(const Hyperion::Vector2 &vector) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["x"] = vector.x;
    node["y"] = vector.y;
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Vector2>::decode(const Node &node, Hyperion::Vector2 &vector) {
    if (!node.IsMap() || node.size() != 2) {
      return false;
    }

    auto yaml_x = node["x"];
    if (!yaml_x || !yaml_x.IsScalar()) {
      return false;
    }
    vector.x = yaml_x.as<float32>();

    auto yaml_y = node["y"];
    if (!yaml_y || !yaml_y.IsScalar()) {
      return false;
    }
    vector.y = yaml_y.as<float32>();

    return true;
  }

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector2 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  Node convert<Hyperion::Vector3>::encode(const Hyperion::Vector3 &vector) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["x"] = vector.x;
    node["y"] = vector.y;
    node["z"] = vector.z;
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Vector3>::decode(const Node &node, Hyperion::Vector3 &vector) {
    if (!node.IsMap() || node.size() != 3) {
      return false;
    }

    auto yaml_x = node["x"];
    if (!yaml_x || !yaml_x.IsScalar()) {
      return false;
    }
    vector.x = yaml_x.as<float32>();

    auto yaml_y = node["y"];
    if (!yaml_y || !yaml_y.IsScalar()) {
      return false;
    }
    vector.y = yaml_y.as<float32>();

    auto yaml_z = node["z"];
    if (!yaml_z || !yaml_z.IsScalar()) {
      return false;
    }
    vector.z = yaml_z.as<float32>();

    return true;
  }
  
  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector3 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << Key << "z" << Value << vector.z;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  Node convert<Hyperion::Vector4>::encode(const Hyperion::Vector4 &vector) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["x"] = vector.x;
    node["y"] = vector.y;
    node["z"] = vector.z;
    node["w"] = vector.w;
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Vector4>::decode(const Node &node, Hyperion::Vector4 &vector) {
    if (!node.IsMap() || node.size() != 4) {
      return false;
    }

    auto yaml_x = node["x"];
    if (!yaml_x || !yaml_x.IsScalar()) {
      return false;
    }
    vector.x = yaml_x.as<float32>();

    auto yaml_y = node["y"];
    if (!yaml_y || !yaml_y.IsScalar()) {
      return false;
    }
    vector.y = yaml_y.as<float32>();

    auto yaml_z = node["z"];
    if (!yaml_z || !yaml_z.IsScalar()) {
      return false;
    }
    vector.z = yaml_z.as<float32>();

    auto yaml_w = node["w"];
    if (!yaml_w || !yaml_w.IsScalar()) {
      return false;
    }
    vector.w = yaml_w.as<float32>();

    return true;
  }

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Vector4 &vector) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << vector.x;
    emitter << Key << "y" << Value << vector.y;
    emitter << Key << "z" << Value << vector.z;
    emitter << Key << "w" << Value << vector.w;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  Node convert<Hyperion::Quaternion>::encode(const Hyperion::Quaternion &quaternion) {
    Node node;
    node.SetStyle(EmitterStyle::Flow);
    node["x"] = quaternion.x;
    node["y"] = quaternion.y;
    node["z"] = quaternion.z;
    node["w"] = quaternion.w;
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Quaternion>::decode(const Node &node, Hyperion::Quaternion &quaternion) {
    if (!node.IsMap() || node.size() != 4) {
      return false;
    }

    auto yaml_x = node["x"];
    if (!yaml_x || !yaml_x.IsScalar()) {
      return false;
    }
    quaternion.x = yaml_x.as<float32>();

    auto yaml_y = node["y"];
    if (!yaml_y || !yaml_y.IsScalar()) {
      return false;
    }
    quaternion.y = yaml_y.as<float32>();

    auto yaml_z = node["z"];
    if (!yaml_z || !yaml_z.IsScalar()) {
      return false;
    }
    quaternion.z = yaml_z.as<float32>();

    auto yaml_w = node["w"];
    if (!yaml_w || !yaml_w.IsScalar()) {
      return false;
    }
    quaternion.w = yaml_w.as<float32>();

    return true;
  }

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Quaternion &quaternion) {
    emitter << Flow;
    emitter << BeginMap;
    emitter << Key << "x" << Value << quaternion.x;
    emitter << Key << "y" << Value << quaternion.y;
    emitter << Key << "z" << Value << quaternion.z;
    emitter << Key << "w" << Value << quaternion.w;
    emitter << EndMap;
    return emitter;
  }

  //--------------------------------------------------------------
  Node convert<Hyperion::Matrix4x4>::encode(const Hyperion::Matrix4x4 &matrix) {
    Node node;
    node["row1"] = matrix.GetRow(0);
    node["row2"] = matrix.GetRow(1);
    node["row3"] = matrix.GetRow(2);
    node["row4"] = matrix.GetRow(3);
    return node;
  }

  //--------------------------------------------------------------
  bool8 convert<Hyperion::Matrix4x4>::decode(const Node &node, Hyperion::Matrix4x4 &matrix) {
    if (!node.IsMap() || node.size() != 4) {
      return false;
    }

    auto yaml_r = node["row1"];
    if (!yaml_r || !yaml_r.IsMap()) {
      return false;
    }
    matrix.SetRow(0, yaml_r.as<Hyperion::Vector4>());

    auto yaml_g = node["row2"];
    if (!yaml_g || !yaml_g.IsMap()) {
      return false;
    }
    matrix.SetRow(1, yaml_r.as<Hyperion::Vector4>());

    auto yaml_b = node["row3"];
    if (!yaml_b || !yaml_b.IsMap()) {
      return false;
    }
    matrix.SetRow(2, yaml_r.as<Hyperion::Vector4>());

    auto yaml_a = node["row4"];
    if (!yaml_a || !yaml_a.IsMap()) {
      return false;
    }
    matrix.SetRow(3, yaml_r.as<Hyperion::Vector4>());

    return true;
  }

  //--------------------------------------------------------------
  Emitter &operator <<(Emitter &emitter, const Hyperion::Matrix4x4 &matrix) {
    emitter << BeginMap;
    emitter << Key << "row1" << Value << matrix.GetRow(0);
    emitter << Key << "row2" << Value << matrix.GetRow(1);
    emitter << Key << "row3" << Value << matrix.GetRow(2);
    emitter << Key << "row4" << Value << matrix.GetRow(3);
    emitter << EndMap;
    return emitter;
  }

}
