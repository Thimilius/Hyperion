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
        RenderTexture,
        Shader,
        Texture,
        TextureAtlas,
    };

    class Asset : public Object {
    public:
        virtual ~Asset() override = default;
    public:
        virtual AssetType GetAssetType() const = 0;

        inline ResourceId GetResourceId() const { return m_resource_id; }
    protected:
        Asset();
    protected:
        virtual void OnClone(Object *clone) override;
        virtual void OnDestroy() override;

        void RegisterAsset();
    protected:
        ResourceId m_resource_id;
    };

}