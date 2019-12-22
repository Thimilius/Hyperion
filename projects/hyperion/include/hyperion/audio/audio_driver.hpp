#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Audio {

    class AudioDriver {
    public:
        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void Load(const String &name, const String &path) = 0;
        virtual void Play(const String &name) = 0;
    };

    class DummyAudioDriver : public AudioDriver {
    public:
        virtual void Init() override { }
        virtual void Shutdown() override { }

        virtual void Load(const String &name, const String &path) override { }
        virtual void Play(const String &name) override { }
    };

}