//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/loader/image_loader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/stb/stb_image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Array<String> ImageLoader::GetSupportedExtensions() {
    Array<String> extensions;

    return extensions;
  }

  //--------------------------------------------------------------
  bool8 ImageLoader::SupportsExtension(const String &extension) {
    for (IImageLoader *image_loader : s_loaders) {
      if (image_loader->SupportsExtension(extension)) {
        return true;
      }
    }
    return false;
  }

  //--------------------------------------------------------------
  Result<Image *, Error> ImageLoader::Load(const String &path, bool8 flip_vertically) {
    for (IImageLoader *image_loader : s_loaders) {
      if (image_loader->SupportsExtension(StringUtils::GetExtension(path))) {
        return image_loader->Load(path, flip_vertically);
      }
    }
    return { Error::NoLoaderAvailable };
  }

  //--------------------------------------------------------------
  void ImageLoader::AddFormatLoader(IImageLoader *image_loader) {
    s_loaders.Add(image_loader);
  }

  //--------------------------------------------------------------
  void ImageLoader::Initialize() {
    AddFormatLoader(new StbImageLoader());
  }

  //--------------------------------------------------------------
  void ImageLoader::Shutdown() {
    for (IImageLoader *image_loader : s_loaders) {
      delete image_loader;
    }
  }

}