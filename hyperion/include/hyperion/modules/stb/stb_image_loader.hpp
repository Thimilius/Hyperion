//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class StbImageLoader : public IImageLoader {
  public:
    inline const Array<String> &GetSupportedExtensions() const override { return s_supported_extensions; }
    bool8 SupportsExtension(const String &extension) const override;

    Result<Image *, Error> Load(const String &path, bool8 flip_vertically) override;
  private:
    inline static Array<String> s_supported_extensions = {
      ".png",
      ".jpg",
      ".jpeg",
      ".gif",
      ".bmp",
      ".tga",
      ".psd"
    };
  };

}
