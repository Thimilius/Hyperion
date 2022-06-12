//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_storage.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/opengl/opengl_debug_group.hpp"
#include "hyperion/render/driver/opengl/opengl_shader_compiler.hpp"
#include "hyperion/render/driver/opengl/opengl_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void OpenGLStorage::Initialize() {
    {
      const char *error_vertex = R"(
        #version 450 core

        layout(location = 0) in vec3 a_position;

        layout(std140, binding = 0) uniform Camera {
	        mat4 view;
	        mat4 projection;
        } u_camera;

        uniform mat4 u_model;

        vec4 obj_to_clip_space(vec3 position) {
	        return u_camera.projection * u_camera.view * u_model * vec4(position, 1.0);
        }

        void main() {
	        gl_Position = obj_to_clip_space(a_position);
        }
      )";
      const char *error_fragment = R"(
        #version 450 core

        layout(location = 0) out vec4 o_color;

        void main() {
	        o_color = vec4(1, 0, 1, 1);
        }
      )";
      m_static.error_shader.program = OpenGLShaderCompiler::Compile(error_vertex, error_fragment).program;
      m_static.error_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::Model)] =
        glGetUniformLocation(m_static.error_shader.program, "u_model");
    }

    {
      const char *object_id_vertex = R"(
        #version 450 core

        layout(location = 0) in vec3 a_position;

        layout(std140, binding = 0) uniform Camera {
	        mat4 view;
	        mat4 projection;
        } u_camera;

        uniform mat4 u_model;

        vec4 obj_to_clip_space(vec3 position) {
	        return u_camera.projection * u_camera.view * u_model * vec4(position, 1.0);
        }

        void main() {
	        gl_Position = obj_to_clip_space(a_position);
        }
      )";
      const char *object_id_fragment = R"(
        #version 450 core

        layout(location = 0) out uint o_object_id;

        uniform uint u_object_id;

        void main() {
	        o_object_id = u_object_id;
        }
      )";
      m_static.object_id_shader.program = OpenGLShaderCompiler::Compile(object_id_vertex, object_id_fragment).program;
      m_static.object_id_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::Model)] =
        glGetUniformLocation(m_static.object_id_shader.program, "u_model");
      m_static.object_id_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::ObjectId)] =
        glGetUniformLocation(m_static.object_id_shader.program, "u_object_id");
    }

    {
      const char *fullscreen_vertex = R"(
        #version 450 core

        out V2F {
	        vec2 texture0;
        } o_v2f;

        void main() {
	        vec2 vertices[3] = vec2[3](vec2(-1.0, -1.0f), vec2(-1.0, 3.0), vec2(3.0f, -1.0));
	        vec4 position = vec4(vertices[gl_VertexID], 0.0, 1.0);

	        o_v2f.texture0 = 0.5 * position.xy + vec2(0.5);

	        gl_Position = position;
        }
      )";
      const char *fullscreen_fragment = R"(
        #version 450 core

        layout(location = 0) out vec4 o_color;

        in V2F {
	        vec2 texture0;
        } i_v2f;

        uniform sampler2D u_texture;

        void main() {
	        o_color = texture(u_texture, i_v2f.texture0);
        }
      )";
      m_static.fullscreen_shader = OpenGLShaderCompiler::Compile(fullscreen_vertex, fullscreen_fragment).program;
      glCreateVertexArrays(1, &m_static.fullscreen_vertex_array);
    }

    {
      glCreateBuffers(1, &m_static.camera_uniform_buffer);
      glNamedBufferData(m_static.camera_uniform_buffer, sizeof(OpenGLUniformBufferCamera), nullptr, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_static.camera_uniform_buffer);
    }

    {
      glCreateBuffers(1, &m_static.render_bounds_vertex_buffer);
      glNamedBufferData(m_static.render_bounds_vertex_buffer, 8 * sizeof(OpenGLImmediateVertex), nullptr, GL_DYNAMIC_DRAW);

      Array<uint32> indices = {
        0, 1,
        1, 5,
        5, 4,
        4, 0,

        0, 2,
        1, 3,
        5, 7,
        4, 6,

        2, 3,
        3, 7,
        7, 6,
        6, 2,
      };

      glCreateBuffers(1, &m_static.render_bounds_index_buffer);
      glNamedBufferData(m_static.render_bounds_index_buffer, indices.GetLength() * sizeof(uint32), indices.GetData(), GL_STATIC_DRAW);

      glCreateVertexArrays(1, &m_static.render_bounds_vertex_array);
      glVertexArrayVertexBuffer(m_static.render_bounds_vertex_array, 0, m_static.render_bounds_vertex_buffer, 0, sizeof(OpenGLImmediateVertex));
      glVertexArrayElementBuffer(m_static.render_bounds_vertex_array, m_static.render_bounds_index_buffer);

      glEnableVertexArrayAttrib(m_static.render_bounds_vertex_array, 0);
      glVertexArrayAttribFormat(m_static.render_bounds_vertex_array, 0, 3, GL_FLOAT, false, 0);
      glVertexArrayAttribBinding(m_static.render_bounds_vertex_array, 0, 0);
      glEnableVertexArrayAttrib(m_static.render_bounds_vertex_array, 3);
      glVertexArrayAttribFormat(m_static.render_bounds_vertex_array, 3, 4, GL_FLOAT, false, sizeof(Vector3));
      glVertexArrayAttribBinding(m_static.render_bounds_vertex_array, 3, 0);
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadAssets(RenderAssetContext &asset_context) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadAssets")
    OpenGLDebugGroup debug_group("LoadAssets");

    // The order in which we load the assets is important.
    // For example a material might reference a shader or texture which should always be loaded first.

    for (RenderAssetShader &shader : asset_context.GetShaderAssetsToLoad()) {
      LoadShader(shader);
    }
    for (RenderAssetTexture2D &texture_2d : asset_context.GetTexture2DAssetsToLoad()) {
      LoadTexture2D(texture_2d);
    }
    for (RenderAssetRenderTexture &render_texture : asset_context.GetRenderTextureAssetsToLoad()) {
      LoadRenderTexture(render_texture);
    }
    for (RenderAssetMaterial &material : asset_context.GetMaterialAssetsToLoad()) {
      LoadMaterial(material);
    }
    for (RenderAssetMesh &mesh : asset_context.GetMeshAssetsToLoad()) {
      LoadMesh(mesh);
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadTexture2D(RenderAssetTexture2D &texture_2d) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadTexture2D")

    OpenGLTexture opengl_texture = { };
    opengl_texture.handle = texture_2d.handle;

    glCreateTextures(GL_TEXTURE_2D, 1, &opengl_texture.texture);

    SetTextureAttributes(opengl_texture.texture, texture_2d.parameters.attributes);

    TextureFormat format = texture_2d.parameters.format;
    GLsizei width = texture_2d.parameters.width;
    GLsizei height = texture_2d.parameters.height;
    GLenum internal_format = OpenGLUtilities::GetTextureInternalFormat(format);
    GLsizei mipmap_count = Math::Max(texture_2d.mipmap_count, 1);
    glTextureStorage2D(opengl_texture.texture, mipmap_count, internal_format, width, height);

    OpenGLUtilities::FlipTextureHorizontally(texture_2d.parameters.width, texture_2d.parameters.height, format, texture_2d.pixels);
    GLenum format_value = OpenGLUtilities::GetTextureFormat(format);
    GLenum format_type = OpenGLUtilities::GetTextureFormatType(format);
    OpenGLUtilities::SetUnpackAlignmentForTextureFormat(format);
    glTextureSubImage2D(opengl_texture.texture, 0, 0, 0, width, height, format_value, format_type, texture_2d.pixels.GetData());

    if (texture_2d.parameters.attributes.use_mipmaps) {
      glGenerateTextureMipmap(opengl_texture.texture);
    }

    m_textures.Insert(texture_2d.handle, opengl_texture);
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadRenderTexture(RenderAssetRenderTexture &render_texture) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadRenderTexture")

    if (m_render_textures.Contains(render_texture.handle)) {
      UnloadRenderTexture(render_texture.handle);
    }

    // TODO: We should do some sort of validation.

    uint64 attachment_count = render_texture.parameters.attachments.GetLength();
    uint32 width = render_texture.parameters.width;
    uint32 height = render_texture.parameters.height;

    OpenGLRenderTexture opengl_render_texture;
    opengl_render_texture.handle = render_texture.handle;
    opengl_render_texture.attachments.Resize(attachment_count);
    opengl_render_texture.width = width;
    opengl_render_texture.height = height;

    glCreateFramebuffers(1, &opengl_render_texture.framebuffer);

    uint32 color_attachment_index = 0;
    for (uint32 i = 0; i < attachment_count; i++) {
      const RenderTextureAttachment &attachment = render_texture.parameters.attachments[i];
      RenderTextureFormat format = attachment.format;

      OpenGLRenderTextureAttachment &opengl_attachment = opengl_render_texture.attachments[i];
      opengl_attachment.format = format;

      GLenum opengl_attachment_type = OpenGLUtilities::GetRenderTextureAttachmentType(format);
      // For a color attachment we will always get GL_COLOR_ATTACHMENT0.
      // This means we need to get the proper index for the color attachment.
      if (OpenGLUtilities::IsRenderTextureFormatAColor(format)) {
        opengl_attachment_type += color_attachment_index;
        color_attachment_index++;
      }
      GLenum opengl_internal_format = OpenGLUtilities::GetRenderTextureInternalFormat(format);

      // A readable attachment will be created as a texture otherwise as a renderbuffer.
      if (attachment.readable) {
        glCreateTextures(GL_TEXTURE_2D, 1, &opengl_attachment.attachment);

        TextureAttributes attributes = attachment.attributes;
        SetTextureAttributes(opengl_attachment.attachment, attributes);

        GLsizei mipmap_count = attributes.use_mipmaps ? Math::Max(render_texture.mipmap_count, 1) : 1;
        glTextureStorage2D(opengl_attachment.attachment, mipmap_count, opengl_internal_format, width, height);

        glNamedFramebufferTexture(opengl_render_texture.framebuffer, opengl_attachment_type, opengl_attachment.attachment, 0);
      } else {
        glCreateRenderbuffers(1, &opengl_attachment.attachment);
        glNamedRenderbufferStorage(opengl_attachment.attachment, opengl_internal_format, width, height);
        glNamedFramebufferRenderbuffer(opengl_render_texture.framebuffer, opengl_attachment_type, GL_RENDERBUFFER, opengl_attachment.attachment);
      }
    }

    // For now we specify that we want to draw into all color attachments of the render texture.
    uint32 color_attachment_count = color_attachment_index;
    Array<GLenum> buffers(color_attachment_count);
    for (GLenum i = 0; i < color_attachment_count; i++) {
      buffers[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glNamedFramebufferDrawBuffers(opengl_render_texture.framebuffer, color_attachment_count, buffers.GetData());

    if (glCheckNamedFramebufferStatus(opengl_render_texture.framebuffer, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
      m_render_textures.Insert(render_texture.handle, opengl_render_texture);
    } else {
      HYP_LOG_ERROR("OpenGL", "Failed to create render texture!");
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadShader(RenderAssetShader &shader) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadShader")

    OpenGLShader opengl_shader;
    opengl_shader.handle = shader.handle;
    opengl_shader.attributes = shader.data.attributes;

    if (shader.is_valid) {
      OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(shader.data.vertex_source.c_str(), shader.data.fragment_source.c_str());
      if (compilation_result.success) {
        opengl_shader.program = compilation_result.program;

        opengl_shader.material_locations.Reserve(shader.data.properties.GetLength());
        for (const ShaderProperty &property : shader.data.properties) {
          opengl_shader.material_locations.Add(glGetUniformLocation(opengl_shader.program, property.name.c_str()));
        }
        
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::Model)] = glGetUniformLocation(opengl_shader.program, "u_model");
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::Color)] = glGetUniformLocation(opengl_shader.program, "u_color");
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::Texture)] = glGetUniformLocation(opengl_shader.program, "u_texture");
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::LightCount)] =
          glGetUniformLocation(opengl_shader.program, "u_light_count");
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::LightIndices)] =
          glGetUniformLocation(opengl_shader.program, "u_light_indices");
        opengl_shader.fixed_locations[static_cast<uint32>(OpenGLShaderUniformLocation::ObjectId)] = glGetUniformLocation(opengl_shader.program, "u_object_id");
      } else {
        opengl_shader.program = m_static.error_shader.program;
      }
    } else {
      opengl_shader.program = m_static.error_shader.program;
    }

    auto shader_it = m_shaders.Find(shader.handle);
    if (shader_it == m_shaders.end()) {
      m_shaders.Insert(shader.handle, opengl_shader);
    } else {
      shader_it->second = opengl_shader;
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadMaterial(RenderAssetMaterial &material) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadMaterial")

    auto material_it = m_materials.Find(material.handle);
    if (material_it == m_materials.end()) {
      OpenGLMaterial opengl_material;
      opengl_material.handle = material.handle;
      opengl_material.shader = &m_shaders.Get(material.shader_handle);;
      opengl_material.properties = std::move(material.properties);

      m_materials.Insert(material.handle, opengl_material);
    } else {
      OpenGLMaterial &opengl_material = material_it->second;
      opengl_material.properties = std::move(material.properties);
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::LoadMesh(RenderAssetMesh &mesh) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.LoadMesh")

    const MeshData &data = mesh.data;
    const MeshVertexFormat &vertex_format = mesh.vertex_format;

    bool8 has_normals = data.normals.GetLength() > 0;
    bool8 has_colors = data.colors.GetLength() > 0;
    bool8 has_texture0 = data.texture0.GetLength() > 0;

    // Create interleaved data for better access on the GPU.
    uint32 vertex_count = static_cast<uint32>(data.positions.GetLength());
    Array<byte> vertices(vertex_count * vertex_format.stride);
    for (uint32 i = 0; i < vertex_count; i++) {
      uint32 index = i * vertex_format.stride;

      Vector3 &position = reinterpret_cast<Vector3 &>(vertices[index]);
      position = data.positions[i];
      index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_POSITION;

      if (has_normals) {
        Vector3 &normal = reinterpret_cast<Vector3 &>(vertices[index]);
        normal = data.normals[i];
        index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_NORMAL;
      }
      if (has_colors) {
        Vector4 &color = reinterpret_cast<Vector4 &>(vertices[index]);
        color = data.colors[i];
        index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_COLOR;
      }
      if (has_texture0) {
        Vector2 &texture0 = reinterpret_cast<Vector2 &>(vertices[index]);
        texture0 = data.texture0[i];
        index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
      }
    }

    uint64 vertex_buffer_size = vertices.GetLength();
    uint64 index_buffer_size = data.indices.GetLength() * sizeof(data.indices[0]);
    
    auto mesh_it = m_meshes.Find(mesh.handle);
    if (mesh_it == m_meshes.end()) {
      OpenGLMesh opengl_mesh;
      opengl_mesh.handle = mesh.handle;
      opengl_mesh.sub_meshes = std::move(mesh.sub_meshes);
      opengl_mesh.vertex_buffer_size = vertex_buffer_size;
      opengl_mesh.index_buffer_size = index_buffer_size;

      glCreateBuffers(1, &opengl_mesh.vertex_buffer);
      glNamedBufferData(opengl_mesh.vertex_buffer, vertex_buffer_size, vertices.GetData(), GL_STATIC_DRAW);

      glCreateBuffers(1, &opengl_mesh.index_buffer);
      glNamedBufferData(opengl_mesh.index_buffer, index_buffer_size, data.indices.GetData(), GL_STATIC_DRAW);
      opengl_mesh.index_count = static_cast<GLsizei>(data.indices.GetLength());

      GLuint binding_index = 0;
      GLsizei stride = vertex_format.stride;
      GLuint relative_offset = 0;
      glCreateVertexArrays(1, &opengl_mesh.vertex_array);
      glVertexArrayVertexBuffer(opengl_mesh.vertex_array, binding_index, opengl_mesh.vertex_buffer, 0, stride);
      glVertexArrayElementBuffer(opengl_mesh.vertex_array, opengl_mesh.index_buffer);

      for (VertexAttribute vertex_attribute : vertex_format.attributes) {
        GLuint attribute_index = OpenGLUtilities::GetAttributeIndexForVertextAttributeSize(vertex_attribute.kind);
        GLint size = vertex_attribute.dimension;
        GLenum type = OpenGLUtilities::GetVertexAttributeType(vertex_attribute.type);

        glEnableVertexArrayAttrib(opengl_mesh.vertex_array, attribute_index);
        glVertexArrayAttribFormat(opengl_mesh.vertex_array, attribute_index, size, type, false, relative_offset);
        glVertexArrayAttribBinding(opengl_mesh.vertex_array, attribute_index, binding_index);

        relative_offset += OpenGLUtilities::GetVertexAttributeSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
      }

      m_meshes.Insert(mesh.handle, opengl_mesh);
    } else {
      // FIXME: We are not updating a potentially changed vertex format of the mesh.
      // For that we would need to create the vertex array as well.

      OpenGLMesh &opengl_mesh = mesh_it->second;
      opengl_mesh.sub_meshes = std::move(mesh.sub_meshes);

      if (vertex_buffer_size > opengl_mesh.vertex_buffer_size) {
        glNamedBufferData(opengl_mesh.vertex_buffer, vertex_buffer_size, vertices.GetData(), GL_STATIC_DRAW);
      } else {
        glNamedBufferSubData(opengl_mesh.vertex_buffer, 0, vertex_buffer_size, vertices.GetData());
      }
      
      if (index_buffer_size > opengl_mesh.index_buffer_size) {
        glNamedBufferData(opengl_mesh.index_buffer, index_buffer_size, data.indices.GetData(), GL_STATIC_DRAW);
      } else {
        glNamedBufferSubData(opengl_mesh.index_buffer, 0, index_buffer_size, data.indices.GetData());
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::UnloadAssets(RenderAssetContext &asset_context) {
    HYP_PROFILE_SCOPE("OpenGLRenderDriver.UnloadAssets")
    OpenGLDebugGroup debug_group("UnloadAssets");

    for (AssetHandle shader_handle : asset_context.GetShaderAssetsToUnload()) {
      glDeleteProgram(m_shaders.Get(shader_handle).program);
      m_shaders.Remove(shader_handle);
    }
    for (AssetHandle texture_2d_handle : asset_context.GetTexture2DAssetsToUnload()) {
      GLuint texture = m_textures.Get(texture_2d_handle).texture;
      glDeleteTextures(1, &texture);
      m_textures.Remove(texture_2d_handle);
    }
    for (AssetHandle render_texture_handle : asset_context.GetRenderTextureAssetsToUnload()) {
      UnloadRenderTexture(render_texture_handle);
    }
    for (AssetHandle material_handle : asset_context.GetMaterialAssetsToUnload()) {
      m_materials.Remove(material_handle);
    }
    for (AssetHandle mesh_handle : asset_context.GetMeshAssetsToUnload()) {
      auto mesh_it = m_meshes.Find(mesh_handle);
      if (mesh_it == m_meshes.end()) {
        HYP_LOG_ERROR("OpenGL", "Trying to delete mesh {} which does not exist!", mesh_handle);
        HYP_DEBUG_BREAK;
      } else {
        OpenGLMesh &opengl_mesh = mesh_it->second;

        glDeleteBuffers(1, &opengl_mesh.vertex_buffer);
        glDeleteBuffers(1, &opengl_mesh.index_buffer);
        glDeleteVertexArrays(1, &opengl_mesh.vertex_array);

        m_meshes.Remove(mesh_handle);
      }
    }
  }

  //--------------------------------------------------------------
  void OpenGLStorage::UnloadRenderTexture(AssetHandle render_texture_handle) {
    OpenGLRenderTexture &opengl_render_texture = m_render_textures.Get(render_texture_handle);
    for (OpenGLRenderTextureAttachment &opengl_attachment : opengl_render_texture.attachments) {
      if (opengl_attachment.format == RenderTextureFormat::Depth24Stencil8) {
        glDeleteRenderbuffers(1, &opengl_attachment.attachment);
      } else {
        glDeleteTextures(1, &opengl_attachment.attachment);
      }
    }
    glDeleteFramebuffers(1, &opengl_render_texture.framebuffer);

    m_render_textures.Remove(render_texture_handle);
  }

  //--------------------------------------------------------------
  void OpenGLStorage::SetTextureAttributes(GLuint texture, TextureAttributes attributes) {
    GLint wrap_mode = OpenGLUtilities::GetTextureWrapMode(attributes.wrap_mode);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, wrap_mode);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, wrap_mode);

    GLint min_filter = OpenGLUtilities::GetTextureMinFilter(attributes.filter);
    GLint mag_filter = OpenGLUtilities::GetTextureMaxFilter(attributes.filter);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, min_filter);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, mag_filter);

    GLfloat anisotropic_filter_value = OpenGLUtilities::GetTextureAnisotropicFilter(attributes.anisotropic_filter);
    glTextureParameterf(texture, GL_TEXTURE_MAX_ANISOTROPY, anisotropic_filter_value);
  }

}
