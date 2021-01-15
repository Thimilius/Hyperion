#pragma once

#include "hyperion/assets/asset.hpp"

namespace Hyperion {

    class Shader : public Asset {
    public:
        inline AssetType GetAssetType() const override { return AssetType::Shader; }

        static Shader *Create(const String &vertex, const String &fragment);
    private:
        Shader(const String &vertex, const String &fragment);
        ~Shader() override;
    };

}