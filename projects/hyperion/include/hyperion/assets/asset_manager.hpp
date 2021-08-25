#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssetManager {
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        friend class Hyperion::Engine;
    };

}