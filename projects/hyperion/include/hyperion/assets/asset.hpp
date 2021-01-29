#pragma once

#include "hyperion/core/object/object.hpp"
#include "hyperion/core/object/resource.hpp"

namespace Hyperion {
    class Engine;
}

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

    class AssetManager final {
    private:
        ~AssetManager() = delete;
        AssetManager() = delete;

        static void Initialize();
        static void Shutdown();
    private:
        friend class Engine;
    };
    
}