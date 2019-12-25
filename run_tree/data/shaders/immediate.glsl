#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec4 a_color;
layout(location = 3) in vec2 a_uv;

out VS_TO_FS {
	vec4 color;
} out_vs_to_fs;

uniform struct Transform {
	mat4 view;
	mat4 projection;
} u_transform;

void main() {
	out_vs_to_fs.color = a_color;

	gl_Position = u_transform.projection * u_transform.view * vec4(a_position, 1.0);
}

#type fragment
#version 410 core

out vec4 o_color;

in VS_TO_FS {
	vec4 color;
} in_vs_to_fs;

void main() {
	o_color = in_vs_to_fs.color;
}