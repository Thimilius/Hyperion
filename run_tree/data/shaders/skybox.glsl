#type vertex
#version 410 core

#import "basic_vertex"

void main() {
	o_v2f.position = a_position;

	gl_Position = (u_transform.projection * mat4(mat3(u_transform.view)) * vec4(a_position, 1.0)).xyww;
}

#type fragment
#version 410 core

#import "basic_fragment"

uniform samplerCube u_skybox;

void main() {
	o_color = texture(u_skybox, i_v2f.position);
}