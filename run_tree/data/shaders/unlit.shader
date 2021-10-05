#property m_color Color

#render_order opaque

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;

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
	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;

uniform vec4 m_color;

void main() {
	o_color = m_color;
}