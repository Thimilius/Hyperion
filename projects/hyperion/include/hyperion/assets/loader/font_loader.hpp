#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/font.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class IFontLoader {
    public:
        virtual ~IFontLoader() = default;
    public:
        virtual Font *LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) = 0;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    };

    class FontLoader final {
    public:
        static Font *LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) { return s_font_loader->LoadFont(path, font_size, character_set); }
    private:
        FontLoader() = delete;
        ~FontLoader() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static IFontLoader *s_font_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}