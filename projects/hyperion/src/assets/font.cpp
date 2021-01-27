#include "hyppch.hpp"

#include "hyperion/assets/font.hpp"

namespace Hyperion {

    Font *Font::Create() {
        return new Font();
    }

    void FontLoader::Init() {

    }

    void FontLoader::Shutdown() {
        delete s_font_loader;
    }

}