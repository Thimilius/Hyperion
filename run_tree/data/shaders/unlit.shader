#property m_color Color

#light_mode forward

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

vec4 obj_to_clip_space(vec3 position) {
	return u_projection * u_view * u_model * vec4(position, 1.0);
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