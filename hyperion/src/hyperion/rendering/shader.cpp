#include "hyppch.hpp"

#include "hyperion/rendering/shader.hpp"

#include <filesystem>

#include "hyperion/platform/opengl/opengl_shader.hpp"
#include "hyperion/io/file_utilities.hpp"

using namespace Hyperion::IO;

namespace Hyperion::Rendering {

    TRef<CShader> CShader::Create(const TString &name, const TString &source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLShader>(name, source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    TRef<CShader> CShader::Create(const TString &name, const TString &vertex_source, const TString &fragment_source) {
        switch (CRenderAPI::GetAPI()) {
            case ERenderAPI::OpenGL: return std::make_shared<COpenGLShader>(name, vertex_source, fragment_source);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    EShaderType CShader::ShaderTypeFromString(const TString &string) {
        if (string == "vertex") {
            return EShaderType::Vertex;
        } else if (string == "fragment") {
            return EShaderType::Fragment;
        } else {
            HYP_ASSERT_MESSAGE(false, "Failed to get shader type from string!"); return EShaderType::None;
        }
    }

    void CShaderLibrary::Init(const TString &path) {
        for (auto &entry : std::filesystem::directory_iterator(path)) {
            auto &path = entry.path();
            if (path.extension() == ".glsl") {
                auto &filename = path.filename().string();
                Load(filename.substr(0, filename.length() - 5), path.string());
            }
        }

        s_watcher = CFileWatcher::Create(path, [](EFileStatus status, const TString &path, const TString &filename, const TString &extension) {
            if (extension == ".glsl") {
                auto name = filename.substr(0, filename.length() - 5);
                Reload(name);
            }
        }, false);
    }

    TRef<CShader> CShaderLibrary::Load(const TString &name, const TString &filepath) {
        TString &source = CFileUtilities::ReadTextFile(filepath);
        TRef<CShader> &shader = CShader::Create(name, source);
        Add(name, filepath, shader);
        return shader;
    }

    void CShaderLibrary::Add(const TString &name, const TString &filepath, const TRef<CShader> &shader) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) == s_shaders.end(), "Trying to add shader that is already in the library!");
        s_shaders[name] = { shader, filepath };
    }

    TRef<CShader> CShaderLibrary::Get(const TString &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to get shader that is not in the library!");
        return s_shaders[name].shader;
    }

    void CShaderLibrary::Update() {
        s_watcher->Update();
    }

    void CShaderLibrary::Reload(const TString &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to reload a shader that is not in the library!");
        SShaderEntry entry = s_shaders[name];
        TString &source = CFileUtilities::ReadTextFile(entry.filepath);
        entry.shader->Recompile(source);
    }

}