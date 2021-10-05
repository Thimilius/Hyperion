#property m_color Color
#property m_texture Texture

#render_order transparent

#culling_mode off
#blending_mode on
#z_write off

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;
layout(location = 4) in vec2 a_texture0;

out V2F {
	vec2 texture0;
} o_v2f;

layout(std140, binding = 0) uniform Camera
{
  mat4 view;
  mat4 projection;
} u_camera;

uniform mat4 u_model;

vec4 obj_to_clip_space(vec3 position) {
	return u_camera.projection * u_camera.view * u_model * vec4(position, 1.0);
}

void main() {
	o_v2f.texture0 = a_texture0;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in V2F {
	vec2 texture0;
} i_v2f;

uniform vec4 m_color;
uniform sampler2D m_texture;

void main() {
	vec4 texture_color = texture(m_texture, i_v2f.texture0);

	o_color = texture_color * m_color;
}