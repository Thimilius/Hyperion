#type vertex
#version 410 core

#import standard_vertex_attributes

out V2F {
	vec3 position;
} o_v2f;

uniform struct Transform {
	mat4 view;
	mat4 projection;
} u_transform;

void main() {
	o_v2f.position = a_position;

	gl_Position = (u_transform.projection * mat4(mat3(u_transform.view)) * vec4(a_position, 1.0)).xyww;
}

#type fragment
#version 410 core

out vec4 o_color;

in V2F {
	vec3 position;
} i_v2f;

uniform samplerCube u_skybox;

void main() {
	o_color = texture(u_skybox, i_v2f.position);
}