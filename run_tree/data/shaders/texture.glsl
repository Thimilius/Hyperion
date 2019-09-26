#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out VS_OUT {
	vec3 normal;
	vec2 uv;
} vs_out;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	vs_out.uv = a_uv;
	vs_out.normal = a_normal;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

out vec4 o_color;

in VS_OUT {
	vec3 normal;
	vec2 uv;
} fs_in;

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, fs_in.uv);
}