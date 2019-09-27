#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_uv;

out VS_OUT {
	vec4 color;
} vs_out;

uniform struct Transform {
	mat4 view;
	mat4 projection;
} u_transform;

void main() {
	vs_out.color = a_color;

	gl_Position = u_transform.projection * u_transform.view * vec4(a_position, 1.0);
}

#type fragment
#version 410 core

out vec4 o_color;

in VS_OUT {
	vec4 color;
} fs_in;

void main() {
	o_color = fs_in.color;
}