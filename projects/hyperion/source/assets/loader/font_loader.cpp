//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/loader/font_loader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/freetype/freetype_font_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void FontLoader::Initialize() {
        s_font_loader = new FreetypeFontLoader();
        s_font_loader->Initialize();
    }

    //--------------------------------------------------------------
    void FontLoader::Shutdown() {
        s_font_loader->Shutdown();
        delete s_font_loader;
    }

}