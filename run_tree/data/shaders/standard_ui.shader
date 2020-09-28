#light_mode none

#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uv;

out V2F {
	vec4 color;
	vec2 uv;
} o_v2f;

uniform struct Transform {
	mat4 projection;
} u_transform;

void main() {
	o_v2f.color = a_color;
	o_v2f.uv    = a_uv;

	gl_Position = u_transform.projection * vec4(a_position.x, a_position.y, 0.0, 1.0);
}

#type fragment
#version 410 core

out vec4 o_color;

in V2F {
	vec4 color;
	vec2 uv;
} i_v2f;

uniform sampler2D u_texture;

void main() {
	o_color = i_v2f.color;
}