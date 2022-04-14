//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/image.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class IImageLoader {
  public:
    virtual ~IImageLoader() = default;
  public:
    virtual const Array<String> &GetSupportedExtensions() const = 0;
    virtual bool8 SupportsExtension(const String &extension) const = 0;

    virtual Result<Image *, Error> Load(const String &path, bool8 flip_vertically) = 0;
  };

  class ImageLoader final {
  public:
    static Array<String> GetSupportedExtensions();
    static bool8 SupportsExtension(const String &extension);

    static Result<Image *, Error> Load(const String &path, bool8 flip_vertically = false);
    static void AddFormatLoader(IImageLoader *image_loader);
  private:
    ImageLoader() = delete;
    ~ImageLoader() = delete;
  private:
    static void Initialize();
    static void Shutdown();
  private:
    inline static Array<IImageLoader *> s_loaders;
  private:
    friend class Hyperion::AssetManager;
  };

}
