#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class GraphicsDeviceContext : public INonCopyable {
    public:
        virtual ~GraphicsDeviceContext() = default;
    };

}