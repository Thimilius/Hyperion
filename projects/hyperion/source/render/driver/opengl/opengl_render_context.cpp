//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_render_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  constexpr bool8 BREAK_ON_ERROR = true;
  constexpr bool8 LOG_EXTENSIONS = false;
  constexpr bool8 LOG_NOTIFICATIONS = false;
  constexpr GLenum LOG_SEVERITY = GL_DEBUG_SEVERITY_HIGH;

  //--------------------------------------------------------------
  void OpenGLRenderContext::Initialize(Window *main_window, const RenderContextDescriptor &descriptor) {
#ifdef HYP_DEBUG
    InitializeDebug();
#endif

    QueryExtensions();
    CheckExtensions();

    QueryProperties();
    QueryLimits();

    HYP_LOG_INFO("OpenGL", "Initialized graphics driver!");
    HYP_LOG_INFO("OpenGL", "{} {}", m_properties.version, m_properties.renderer);
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::Shutdown() {

  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::InitializeDebug() {
    glDebugMessageCallback(DebugMessageCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::QueryExtensions() {
    int32 extension_count;
    glGetIntegerv(GL_NUM_EXTENSIONS, &extension_count);
    for (int32 i = 0; i < extension_count; i++) {
      String extension = reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i));
      m_extensions.Add(extension);
      if constexpr (LOG_EXTENSIONS) {
        HYP_LOG_INFO("OpenGL", "Extension: '{}' available!", extension);
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::QueryProperties() {
    m_properties.vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    m_properties.renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    m_properties.version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::QueryLimits() {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint *>(&m_limits.max_texture_units));
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, reinterpret_cast<GLint *>(&m_limits.max_texture_size));
    glGetIntegerv(GL_MAX_SAMPLES, reinterpret_cast<GLint *>(&m_limits.max_msaa_samples));
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, reinterpret_cast<GLint *>(&m_limits.max_framebuffer_color_attachments));
    glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, reinterpret_cast<GLint *>(&m_limits.max_framebuffer_width));
    glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, reinterpret_cast<GLint *>(&m_limits.max_framebuffer_height));
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::CheckExtensions() {
    CheckExtension("GL_ARB_direct_state_access");
    CheckExtension("GL_EXT_direct_state_access");
    CheckExtension("GL_ARB_texture_filter_anisotropic");
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::CheckExtension(const String &extension) {
    if (std::find(m_extensions.begin(), m_extensions.end(), extension) == m_extensions.end()) {
      HYP_PANIC_MESSAGE("OpenGL", "Manditory extension: '{}' not available!", extension);
    }
  }

  //--------------------------------------------------------------
  void OpenGLRenderContext::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer) {
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
    if (LOG_SEVERITY >= severity) {
      switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: HYP_LOG_ERROR("OpenGL", log_string_format, "High", source_string, type_string, id, message); break;
        case GL_DEBUG_SEVERITY_MEDIUM: HYP_LOG_WARN("OpenGL", log_string_format, "Medium", source_string, type_string, id, message); break;
        case GL_DEBUG_SEVERITY_LOW: HYP_LOG_WARN("OpenGL", log_string_format, "Low", source_string, type_string, id, message); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
          if constexpr (LOG_NOTIFICATIONS) {
            HYP_LOG_INFO("OpenGL", log_string_format, "Notification", source_string, type_string, id, message);
          }
          break;
        }
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }

    if constexpr (BREAK_ON_ERROR) {
      if (severity == GL_DEBUG_SEVERITY_HIGH) {
        HYP_DEBUG_BREAK;
      }
    }
  }

}