#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include <filesystem>

#include "hyperion/driver/opengl/opengl_shader.hpp"
#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion::Rendering {

    Ref<Shader> Shader::Create(const String &name, const String &source) {
        switch (RenderAPI::GetBackendAPI()) {
            case RenderBackendAPI::OpenGL: return std::make_shared<OpenGLShader>(name, source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Ref<Shader> Shader::Create(const String &name, const String &vertex_source, const String &fragment_source) {
        switch (RenderAPI::GetBackendAPI()) {
            case RenderBackendAPI::OpenGL: return std::make_shared<OpenGLShader>(name, vertex_source, fragment_source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    ShaderType Shader::ShaderTypeFromString(const String &string) {
        if (string == "vertex") {
            return ShaderType::Vertex;
        } else if (string == "fragment") {
            return ShaderType::Fragment;
        } else {
            HYP_ASSERT_MESSAGE(false, "Failed to get shader type from string!"); return ShaderType::None;
        }
    }

    void ShaderLibrary::Init(const String &path) {
        for (auto &entry : std::filesystem::directory_iterator(path)) {
            auto &path = entry.path();
            if (path.extension() == ".glsl") {
                auto &filename = path.filename().string();
                Load(filename.substr(0, filename.length() - 5), path.string());
            }
        }

        s_watcher = FileWatcher::Create(path, [](FileStatus status, const String &path, const String &filename, const String &extension) {
            if (extension == ".glsl") {
                auto name = filename.substr(0, filename.length() - 5);
                Reload(name);
            }
        }, false);
    }

    Ref<Shader> ShaderLibrary::Load(const String &name, const String &filepath) {
        String &source = FileUtilities::ReadTextFile(filepath);
        Ref<Shader> &shader = Shader::Create(name, source);
        Add(name, filepath, shader);
        return shader;
    }

    void ShaderLibrary::Add(const String &name, const String &filepath, const Ref<Shader> &shader) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) == s_shaders.end(), "Trying to add shader that is already in the library!");
        s_shaders[name] = { shader, filepath };
    }

    Ref<Shader> ShaderLibrary::Get(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to get shader that is not in the library!");
        return s_shaders[name].shader;
    }

    void ShaderLibrary::Update() {
        s_watcher->Update();
    }

    void ShaderLibrary::Reload(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to reload a shader that is not in the library!");
        ShaderEntry entry = s_shaders[name];
        String &source = FileUtilities::ReadTextFile(entry.filepath);
        entry.shader->Recompile(source);
    }

}