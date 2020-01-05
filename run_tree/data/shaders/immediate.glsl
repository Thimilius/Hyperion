#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out VS_TO_FS {
	vec4 color;
} o_v2f;

uniform struct Transform {
	mat4 view;
	mat4 projection;
} u_transform;

void main() {
	o_v2f.color = a_color;

	gl_Position = u_transform.projection * u_transform.view * vec4(a_position, 1.0);
}

#type fragment
#version 410 core

out vec4 o_color;

in VS_TO_FS {
	vec4 color;
} i_v2f;

void main() {
	o_color = i_v2f.color;
}