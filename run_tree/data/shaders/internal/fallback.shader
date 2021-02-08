#light_mode none

#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_color;

void main() {
	o_color = vec4(1.0, 0, 1.0, 1.0);
}