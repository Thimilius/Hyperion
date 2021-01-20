#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"

namespace Hyperion {

    class Shader : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        static Shader *Create(const Map<Rendering::ShaderStageFlags, String> &sources);
    private:
        Shader(const Map<Rendering::ShaderStageFlags, String> &sources);
        ~Shader() override;
    };

}