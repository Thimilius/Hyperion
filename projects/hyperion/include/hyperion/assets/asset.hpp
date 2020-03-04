#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class AssetType {
        Mesh,
        Shader,
        Texture,
        Material,
        Font
    };

    class Asset {
    private:
        String m_name;
    public:
        virtual ~Asset() = default;

        virtual AssetType GetAssetType() const = 0;

        inline const String &GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }
    protected:
        Asset() { }
        Asset(const String &name) : m_name(name) { }
    };

}