#light_mode none

#type vertex
#version 410 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_uv;

out V2F {
	vec2 uv;
} o_v2f;

uniform struct Transform {
	mat4 projection;
} u_transform;

void main() {
	o_v2f.uv = a_uv;

	gl_Position = u_transform.projection * vec4(a_position, 0.0, 1.0);
}

#type fragment
#version 410 core

out vec4 o_color;

in V2F {
	vec2 uv;
} i_v2f;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
	vec4 texture_color = vec4(1.0, 1.0, 1.0, texture(u_texture, i_v2f.uv).r);
	
	o_color = u_color * texture_color;
}