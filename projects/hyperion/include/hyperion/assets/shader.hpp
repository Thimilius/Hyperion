#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/shader_descriptor.hpp"

namespace Hyperion {

    class Shader : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        static Shader *Create(const Map<ShaderStageFlags, String> &sources);
    private:
        Shader(const Map<ShaderStageFlags, String> &sources);
        ~Shader() override;
    };

}