#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_graphics_context.hpp"

#define HYP_OPENGL_DEBUG_LOG true
#define HYP_OPENGL_BREAK_ON_ERROR true
#define HYP_OPENGL_LOG_EXTENSIONS false
#define HYP_OPENGL_LOG_NOTIFICATIONS false

namespace Hyperion::Rendering {

    void OpenGLGraphicsContext::Init() {
        // Enable debug messages
        if (HYP_OPENGL_DEBUG_LOG) {
            glDebugMessageCallback(DebugMessageCallback, NULL);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

        // Query extensions
        s32 extension_count;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);
        for (s32 i = 0; i < extension_count; i++) {
            String extension = (char *)glGetStringi(GL_EXTENSIONS, i);
            m_extensions.push_back(extension);
            if (HYP_OPENGL_LOG_EXTENSIONS) {
                HYP_LOG_INFO("OpenGL", "Extension: '{}' available!", extension);
            }
        }
        CheckExtensions();

        // Query properties
        {
            m_properties.vendor = (const char *)glGetString(GL_VENDOR);
            m_properties.renderer = (const char *)glGetString(GL_RENDERER);
            m_properties.version = (const char *)glGetString(GL_VERSION);
        }
        
        // Query limits
        {
            glGetIntegerv(GL_MAX_SAMPLES, &m_limits.max_msaa_samples);
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_limits.max_texture_units);
        }

        HYP_LOG_INFO("OpenGL", "Initialized OpenGL! ({})", m_properties.version);
        HYP_LOG_INFO("OpenGL", "Renderer: {}", m_properties.renderer);
        HYP_LOG_INFO("OpenGL", "Max texture units: {} - Max MSAA samples: {}", m_limits.max_texture_units, m_limits.max_msaa_samples);
    }

    void OpenGLGraphicsContext::CheckExtensions() {
        CheckExtension("GL_ARB_direct_state_access");
        CheckExtension("GL_EXT_direct_state_access");
        CheckExtension("GL_ARB_texture_filter_anisotropic");
    }

    void OpenGLGraphicsContext::CheckExtension(const String &extension) {
        if (std::find(m_extensions.begin(), m_extensions.end(), extension) == m_extensions.end()) {
            HYP_PANIC_MESSAGE("OpenGL", "Manditory OpenGL extension: '{}' not available!", extension);
        }
    }

    void OpenGLGraphicsContext::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer) {
        const char *source_string;
        switch (source) {
            case GL_DEBUG_SOURCE_API:             source_string = "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_string = "Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: source_string = "Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     source_string = "Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     source_string = "Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           source_string = "Other"; break;
            default:                              source_string = "Unknown"; break;
        }

        const char *type_string;
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               type_string = "Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_string = "Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_string = "Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         type_string = "Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         type_string = "Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              type_string = "Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          type_string = "Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           type_string = "Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               type_string = "Other"; break;
            default:                                type_string = "Unknown"; break;
        }

        const char *log_string_format = "Severity: {}, Source: {}, Type: {}, ID: {},\nMessage: {}";
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: HYP_LOG_ERROR("OpenGL", log_string_format, "High", source_string, type_string, id, message); break;
            case GL_DEBUG_SEVERITY_MEDIUM: HYP_LOG_WARN("OpenGL", log_string_format, "Medium", source_string, type_string, id, message); break;
            case GL_DEBUG_SEVERITY_LOW: HYP_LOG_WARN("OpenGL", log_string_format, "Low", source_string, type_string, id, message); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: {
                if (HYP_OPENGL_LOG_NOTIFICATIONS) {
                    HYP_LOG_INFO("OpenGL", log_string_format, "Notification", source_string, type_string, id, message);
                }
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }

        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            if (HYP_OPENGL_BREAK_ON_ERROR) {
                HYP_DEBUG_BREAK;
            }
        }
    }

}