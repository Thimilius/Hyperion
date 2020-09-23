#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Audio {

    class IAudioDriver {
    public:
        virtual ~IAudioDriver() = default;

        virtual void Load(const String &name, const String &path) = 0;
        virtual void Play(const String &name) = 0;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;
    };

    class DummyAudioDriver : public IAudioDriver {
    public:
        virtual void Load(const String &name, const String &path) override { }
        virtual void Play(const String &name) override { }

        virtual void Init() override { HYP_LOG_INFO("Audio", "Initialized dummy audio driver!"); }
        virtual void Shutdown() override { }
    };

}