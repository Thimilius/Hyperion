#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/attributes/shader_attributes.hpp"

namespace Hyperion {

    class Shader : public Asset {
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }
    public:
        static Shader *Create(const String &source);
    protected:
        void OnDestroy() override;
    private:
        Shader() = default;
        Shader(const String &source);
    private:
        static Shader *Create();
    };

}