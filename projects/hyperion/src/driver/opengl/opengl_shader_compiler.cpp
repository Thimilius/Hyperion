#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion::Rendering {

    OpenGLShaderPreProcessResult OpenGLShaderCompiler::PreProcess(const String &source) {
        OpenGLShaderPreProcessResult result;
        result.status = OpenGLShaderPreProcessStatus::Failed;

        Map<ShaderType, String> sources;

        const char *type_token = "#type ";
        const char *import_token = "#import ";
        u64 type_token_length = std::strlen(type_token);
        u64 import_token_length = std::strlen(import_token);

        // TODO: Correctly handle all error cases

        // Find and split the source into corresponding shader type
        u64 position = source.find(type_token, 0);
        while (position != String::npos) {
            String shader_source;
            ShaderType shader_type;

            u64 end_of_line = source.find_first_of("\r\n", position);
            HYP_ASSERT_MESSAGE(end_of_line != String::npos, "Shader syntax error!");
            u64 begin = position + type_token_length;
            String type_string = source.substr(begin, end_of_line - begin);
            shader_type = ShaderTypeFromString(type_string);
            if (shader_type == ShaderType::Unknown) {
                HYP_LOG_ERROR("OpenGL", "Invalid shader type specifier: '{}'!", type_string);
                return result;
            }

            u64 next_line_position = source.find_first_not_of("\r\n", end_of_line);
            position = source.find(type_token, next_line_position);
            u64 end = position - (next_line_position == String::npos ? source.size() - 1 : next_line_position);

            shader_source = source.substr(next_line_position, end);

            // Find and handle import statements in source
            {
                u64 position = shader_source.find(import_token, 0);
                while (position != String::npos) {
                    u64 end_of_line = shader_source.find_first_of("\r\n", position);
                    HYP_ASSERT_MESSAGE(end_of_line != String::npos, "Shader syntax error!");
                    u64 begin = position + import_token_length;
                    u64 end = end_of_line - begin;

                    String import_string = shader_source.substr(begin, end_of_line - begin);
                    if (import_string.find_first_of('\"') != 0) {
                        HYP_LOG_ERROR("OpenGL", "Invalid shader import synatx! Missing start quotation marks!");
                        return result;
                    }
                    u64 import_end = import_string.find_last_of('\"');
                    if (import_end == 0) {
                        HYP_LOG_ERROR("OpenGL", "Invalid shader import synatx! Missing end quotation marks!");
                        return result;
                    }

                    import_string = import_string.substr(1, import_end - 1);
                    String shader_module = ShaderModuleFromString(import_string);
                    if (shader_module.empty()) {
                        HYP_LOG_ERROR("OpenGL", "Invalid shader import module: '{}'!", import_string);
                        return result;
                    }

                    shader_source = shader_source.replace(position, (begin - position) + end, shader_module);

                    position = shader_source.find(import_token, end_of_line);
                }
            }

            sources[shader_type] = shader_source;
        }

        // Check that we found both a vertex and fragment source
        if (sources.find(ShaderType::Vertex) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Failed to find a vertex shader type specifier!");
            return result;
        } else if (sources.find(ShaderType::Fragment) == sources.end()) {
            HYP_LOG_ERROR("OpenGL", "Failed to find a fragment shader type specifier!");
            return result;
        }

        result.status = OpenGLShaderPreProcessStatus::Success;
        result.sources = std::move(sources);

        return result;
    }

    void OpenGLShaderCompiler::Init() {
        // FIXME: There should probably a better and more global way of getting configuration stuff
        for (auto &entry : std::filesystem::directory_iterator(AssetLibrary::GetSettings().shader_path + "/modules")) {
            auto &path = entry.path();
            if (path.extension() == ".glsl_module") {
                String filename = path.filename().u8string();
                String module_name = filename.substr(0, filename.find_last_of('.'));
                s_modules[module_name] = FileUtilities::ReadTextFile(path.u8string());
            }
        }
    }

    ShaderType OpenGLShaderCompiler::ShaderTypeFromString(const String &string) {
        if (string == "vertex") {
            return ShaderType::Vertex;
        } else if (string == "fragment") {
            return ShaderType::Fragment;
        } else {
            return ShaderType::Unknown;
        }
    }

    String OpenGLShaderCompiler::ShaderModuleFromString(const String &string) {
        if (s_modules.find(string) == s_modules.end()) {
            return String();
        }

        return s_modules[string];
    }

}