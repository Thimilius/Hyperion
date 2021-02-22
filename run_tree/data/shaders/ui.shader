#light_mode none

#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 3) in vec4 a_color;
layout(location = 4) in vec2 a_texture0;

out V2F {
	vec4 color;
	vec2 texture0;
} o_v2f;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	o_v2f.color = a_color;
	o_v2f.texture0 = a_texture0;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_color;

in V2F {
	vec4 color;
	vec2 texture0;
} i_v2f;

void main() {
	o_color = i_v2f.color;
}