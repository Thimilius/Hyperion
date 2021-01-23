#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/shader.hpp"

namespace Hyperion {

    class Material : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Material; }

        static Material *Create(Shader *shader);
    private:
        Material(Shader *shader);
        ~Material() override;
    private:
        Shader *m_shader;
    };

}