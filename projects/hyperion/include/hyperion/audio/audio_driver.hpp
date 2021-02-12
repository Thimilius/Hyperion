#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/audio/audio_backend.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Audio {

    class IAudioDriver {
    public:
        virtual ~IAudioDriver() = default;
    public:
        virtual AudioBackend GetBackend() const = 0;

        virtual void Load(const String &name, const String &path) = 0;
        virtual void Play(const String &name) = 0;

        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;
    };

    class DummyAudioDriver : public IAudioDriver {
    public:
        AudioBackend GetBackend() const override { return AudioBackend::None; }

        void Load(const String &name, const String &path) override { }
        void Play(const String &name) override { }

        void Initialize() override { HYP_LOG_INFO("Audio", "Initialized dummy audio driver!"); }
        void Shutdown() override { }
    };

}