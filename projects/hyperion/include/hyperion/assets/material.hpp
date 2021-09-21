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
        inline float32 GetFloat32(const String &name) const { return GetFloat32(Shader::PropertyToId(name)); }
        void SetFloat32(Rendering::MaterialPropertyId id, float32 value);
        inline void SetFloat32(const String &name, float32 value) { SetFloat32(Shader::PropertyToId(name), value); }

        int32 GetInt32(Rendering::MaterialPropertyId id) const;
        inline int32 GetInt32(const String &name) const { return GetInt32(Shader::PropertyToId(name)); }
        void SetInt32(Rendering::MaterialPropertyId id, int32 value);
        inline void SetInt32(const String &name, int32 value) { SetInt32(Shader::PropertyToId(name), value); }

        uint32 GetUInt32(Rendering::MaterialPropertyId id) const;
        inline uint32 GetUInt32(const String &name) const { return GetUInt32(Shader::PropertyToId(name)); }
        void SetUInt32(Rendering::MaterialPropertyId id, uint32 value);
        inline void SetUInt32(const String &name, uint32 value) { SetUInt32(Shader::PropertyToId(name), value); }

        Vector2 GetVector2(Rendering::MaterialPropertyId id) const;
        inline Vector2 GetVector2(const String &name) const { return GetVector2(Shader::PropertyToId(name)); }
        void SetVector2(Rendering::MaterialPropertyId id, Vector2 value);
        inline void SetVector2(const String &name, Vector2 value) { SetVector2(Shader::PropertyToId(name), value); }

        Vector3 GetVector3(Rendering::MaterialPropertyId id) const;
        inline Vector3 GetVector3(const String &name) const { return GetVector3(Shader::PropertyToId(name)); }
        void SetVector3(Rendering::MaterialPropertyId id, Vector3 value);
        inline void SetVector3(const String &name, Vector3 value) { SetVector3(Shader::PropertyToId(name), value); }

        Vector4 GetVector4(Rendering::MaterialPropertyId id) const;
        inline Vector4 GetVector4(const String &name) const { return GetVector4(Shader::PropertyToId(name)); }
        void SetVector4(Rendering::MaterialPropertyId id, Vector4 value);
        inline void SetVector4(const String &name, Vector4 value) { SetVector4(Shader::PropertyToId(name), value); }

        Matrix3x3 GetMatrix3x3(Rendering::MaterialPropertyId id) const;
        inline Matrix3x3 GetMatrix3x3(const String &name) const { return GetMatrix3x3(Shader::PropertyToId(name)); }
        void SetMatrix3x3(Rendering::MaterialPropertyId id, const Matrix3x3 &value);
        inline void SetMatrix3x3(const String &name, const Matrix3x3 &value) { SetMatrix3x3(Shader::PropertyToId(name), value); }

        Matrix4x4 GetMatrix4x4(Rendering::MaterialPropertyId id) const;
        inline Matrix4x4 GetMatrix4x4(const String &name) const { return GetMatrix4x4(Shader::PropertyToId(name)); }
        void SetMatrix4x4(Rendering::MaterialPropertyId id, const Matrix4x4 &value);
        inline void SetMatrix4x4(const String &name, const Matrix4x4 &value) { SetMatrix4x4(Shader::PropertyToId(name), value); }

        Color GetColor(Rendering::MaterialPropertyId id) const;
        inline Color GetColor(const String &name) const { return GetColor(Shader::PropertyToId(name)); }
        void SetColor(Rendering::MaterialPropertyId id, Color value);
        inline void SetColor(const String &name, Color value) { SetColor(Shader::PropertyToId(name), value); }
    private:
        Material(AssetInfo info) : Asset(info) { }
        Material(AssetInfo info, Shader *shader);
    private:
        Shader *m_shader;
        Rendering::MaterialPropertyIndices m_property_indices;
        Rendering::MaterialPropertyCollection m_properties;
    private:
        friend class Hyperion::AssetManager;
    };

}