#pragma once

#include "hyperion/assets/font.hpp"

struct FT_LibraryRec_;

namespace Hyperion {

    class FreetypeFontLoader : public IFontLoader {
    public:
        Font *LoadFont(const String &path, uint32 font_size, FontCharacterSet character_set) override;

        void Initialize() override;
        void Shutdown() override;
    private:
        inline static FT_LibraryRec_ *s_freetype_library;
    };

}