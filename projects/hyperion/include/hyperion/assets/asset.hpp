#pragma once

#include "hyperion/core/object.hpp"

namespace Hyperion {

    using ResourceId = u64;

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
        virtual AssetType GetAssetType() const = 0;

        inline ResourceId GetResourceId() const { return m_resource_id; }
    protected:
        Asset();
        virtual ~Asset() = default;
    protected:
        ResourceId m_resource_id;
    };

    class Resource final {
    private:
        inline static ResourceId GetNextResourceId() { return s_resource_id_counter++; }
    private:
        inline static ResourceId s_resource_id_counter = 0;
    private:
        friend class Hyperion::Asset;
    };

}