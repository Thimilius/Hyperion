//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/font_loader.hpp"

//-------------------- Forward Declarations --------------------
struct FT_LibraryRec_;

//-------------------- Definition Namespace --------------------
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
