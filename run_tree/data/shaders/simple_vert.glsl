#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec4 a_color;

out VS_OUT {
	vec2 uv;
	vec4 color;
} vs_out;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec4 model_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0f);
}

void main() {
	vs_out.uv = a_uv;
	vs_out.color = a_color;

	gl_Position = model_to_clip_space(a_position);
}