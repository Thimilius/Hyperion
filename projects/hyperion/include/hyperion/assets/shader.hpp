#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"

namespace Hyperion {

    class Shader : public Asset {
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        static Shader *Create(const Map<Rendering::ShaderStageFlags, String> &sources);
    protected:
        void OnDestroy() override;
    private:
        Shader() = default;
        Shader(const Map<Rendering::ShaderStageFlags, String> &sources);

        static Shader *Create();
    };

}