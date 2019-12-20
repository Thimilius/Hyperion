#include "hyppch.hpp"

#include "hyperion/audio/audio_engine.hpp"

namespace Hyperion::Audio {

    void CAudioEngine::LoadSound(const TString &name, const TString &path) {
        FMOD::Sound *sound;
        if (m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK) {
            HYP_LOG_ERROR("Audio", "Failed to load sound '{}' at path: '{}'!", name, path);
            return;
        }

        if (m_sounds.find(name) != m_sounds.end()) {
            HYP_LOG_ERROR("Audio", "Sound '{}' already loaded!", name);
        } else {
            m_sounds[name] = sound;
        }
    }

    void CAudioEngine::PlaySound(const TString &name) {
        auto it = m_sounds.find(name);
        if (it == m_sounds.end()) {
            HYP_LOG_ERROR("Audio", "Sound '{}' not loaded!", name);
        } else {
            FMOD::Sound *sound = it->second;
            sound->setMode(FMOD_LOOP_NORMAL);
            sound->setLoopCount(-1);
            m_system->playSound(sound, nullptr, false, nullptr);
        }
    }

    void CAudioEngine::Init() {
        if (FMOD::System_Create(&m_system) != FMOD_OK) {
            HYP_LOG_ERROR("Audio", "Failed to create FMOD system!");
            return;
        }

        s32 driver_count = 0;
        m_system->getNumDrivers(&driver_count);

        if (driver_count == 0) {
            HYP_LOG_ERROR("Audio", "There is no audio driver on this system!");
            return;
        }

        m_system->init(36, FMOD_INIT_NORMAL, nullptr);
        HYP_LOG_INFO("Audio", "Initialized audio engine!");
    }

    void CAudioEngine::Shutdown() {
        for (auto sound : m_sounds) {
            sound.second->release();
        }

        m_system->release();
    }

}