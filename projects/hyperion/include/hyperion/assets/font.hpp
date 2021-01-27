#pragma once

#include "hyperion/assets/asset.hpp"

namespace Hyperion {

    class Font : public Asset {
        HYP_REFLECT(Asset);
    public:
        AssetType GetAssetType() const override { return AssetType::Font; }

        static Font *Create();
    private:
        Font() = default;
    };

    class IFontLoader {
    public:
        virtual ~IFontLoader() = default;

        virtual Font *LoadFont(const String &path) = 0;
    };

    class FontLoader final {
    public:
        static void LoadFont(const String &path) { s_font_loader->LoadFont(path); }
    private:
        FontLoader() = delete;
        ~FontLoader() = delete;

        static void Init();
        static void Shutdown();
    private:
        inline static IFontLoader *s_font_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}