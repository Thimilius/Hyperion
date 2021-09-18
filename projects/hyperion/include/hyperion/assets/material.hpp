#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Material final : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Material; }
        
        inline Shader *GetShader() const { return m_shader; }
        inline const Rendering::MaterialPropertyCollection &GetProperties() const { return m_properties; }

        float32 GetFloat32(Rendering::MaterialPropertyId id) const;
        inline float32 GetFloat32(const String &name) const { return GetFloat32(Rendering::MaterialProperty::NameToId(name)); }
        void SetFloat32(Rendering::MaterialPropertyId id, float32 value);
        inline void SetFloat32(const String &name, float32 value) { SetFloat32(Rendering::MaterialProperty::NameToId(name), value); }

        int32 GetInt32(Rendering::MaterialPropertyId id) const;
        inline int32 GetInt32(const String &name) const { return GetInt32(Rendering::MaterialProperty::NameToId(name)); }
        void SetInt32(Rendering::MaterialPropertyId id, int32 value);
        inline void SetInt32(const String &name, int32 value) { SetInt32(Rendering::MaterialProperty::NameToId(name), value); }

        uint32 GetUInt32(Rendering::MaterialPropertyId id) const;
        inline uint32 GetUInt32(const String &name) const { return GetUInt32(Rendering::MaterialProperty::NameToId(name)); }
        void SetUInt32(Rendering::MaterialPropertyId id, uint32 value);
        inline void SetUInt32(const String &name, uint32 value) { SetUInt32(Rendering::MaterialProperty::NameToId(name), value); }

        Vector2 GetVector2(Rendering::MaterialPropertyId id) const;
        inline Vector2 GetVector2(const String &name) const { return GetVector2(Rendering::MaterialProperty::NameToId(name)); }
        void SetVector2(Rendering::MaterialPropertyId id, Vector2 value);
        inline void SetVector2(const String &name, Vector2 value) { SetVector2(Rendering::MaterialProperty::NameToId(name), value); }

        Vector3 GetVector3(Rendering::MaterialPropertyId id) const;
        inline Vector3 GetVector3(const String &name) const { return GetVector3(Rendering::MaterialProperty::NameToId(name)); }
        void SetVector3(Rendering::MaterialPropertyId id, Vector3 value);
        inline void SetVector3(const String &name, Vector3 value) { SetVector3(Rendering::MaterialProperty::NameToId(name), value); }

        Vector4 GetVector4(Rendering::MaterialPropertyId id) const;
        inline Vector4 GetVector4(const String &name) const { return GetVector4(Rendering::MaterialProperty::NameToId(name)); }
        void SetVector4(Rendering::MaterialPropertyId id, Vector4 value);
        inline void SetVector4(const String &name, Vector4 value) { SetVector4(Rendering::MaterialProperty::NameToId(name), value); }

        Matrix3x3 GetMatrix3x3(Rendering::MaterialPropertyId id) const;
        inline Matrix3x3 GetMatrix3x3(const String &name) const { return GetMatrix3x3(Rendering::MaterialProperty::NameToId(name)); }
        void SetMatrix3x3(Rendering::MaterialPropertyId id, const Matrix3x3 &value);
        inline void SetMatrix3x3(const String &name, const Matrix3x3 &value) { SetMatrix3x3(Rendering::MaterialProperty::NameToId(name), value); }

        Matrix4x4 GetMatrix4x4(Rendering::MaterialPropertyId id) const;
        inline Matrix4x4 GetMatrix4x4(const String &name) const { return GetMatrix4x4(Rendering::MaterialProperty::NameToId(name)); }
        void SetMatrix4x4(Rendering::MaterialPropertyId id, const Matrix4x4 &value);
        inline void SetMatrix4x4(const String &name, const Matrix4x4 &value) { SetMatrix4x4(Rendering::MaterialProperty::NameToId(name), value); }

        Color GetColor(Rendering::MaterialPropertyId id) const;
        inline Color GetColor(const String &name) const { return GetColor(Rendering::MaterialProperty::NameToId(name)); }
        void SetColor(Rendering::MaterialPropertyId id, Color value);
        inline void SetColor(const String &name, Color value) { SetColor(Rendering::MaterialProperty::NameToId(name), value); }
    private:
        Material(AssetInfo info) : Asset(info) { }
        Material(AssetInfo info, Shader *shader);
    private:
        Shader *m_shader;
        Rendering::MaterialPropertyCollection m_properties;
    private:
        friend class Hyperion::AssetManager;
    };

}