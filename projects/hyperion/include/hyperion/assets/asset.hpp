#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"
#include "hyperion/core/object/resource.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class AssetType {
        Font,
        Material,
        Mesh,
        Shader,
        Texture,
        RenderTexture,
        TextureAtlas,
    };

    class Asset : public Object {
        HYP_REFLECT(Object);
    public:
        virtual ~Asset() override = default;
    public:
        virtual AssetType GetAssetType() const = 0;

        inline ResourceId GetResourceId() const { return m_resource_id; }
    protected:
        Asset();
    protected:
        ResourceId m_resource_id;
    };

}