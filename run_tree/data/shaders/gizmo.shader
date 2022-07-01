#render_order opaque

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;
layout(location = 3) in vec4 a_color;

out V2F {
	vec4 color;
} o_v2f;

layout(std140, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} u_camera;

uniform mat4 u_model;

vec4 obj_to_clip_space(vec3 position) {
	return u_camera.projection * u_camera.view * u_model * vec4(position, 1.0);
}

void main() {
	o_v2f.color = a_color;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in V2F {
	vec4 color;
} i_v2f;

uniform vec4 u_add_color;

void main() {
	o_color = i_v2f.color + u_add_color;
}