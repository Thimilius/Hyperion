#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/shader.hpp"

namespace Hyperion {

    class Material : public Asset {
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Material; }

        static Material *Create(Shader *shader);
    protected:
        void OnDestroy() override;
    private:
        Material() = default;
        Material(Shader *shader);

        static Material *Create();
    private:
        Shader *m_shader;
    };

}