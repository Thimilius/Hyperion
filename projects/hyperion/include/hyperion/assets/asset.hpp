#pragma once

#include "hyperion/core/object/object.hpp"
#include "hyperion/core/object/resource.hpp"

namespace Hyperion {

    enum class AssetType {
        Mesh,
        Shader,
        Texture,
        Material,
        Font
    };

    class Asset : public Object {
        HYP_REFLECT(Object);
    public:
        virtual ~Asset() override = default;

        virtual AssetType GetAssetType() const = 0;

        inline ResourceId GetResourceId() const { return m_resource_id; }
    protected:
        Asset();
    protected:
        ResourceId m_resource_id;
    };

}