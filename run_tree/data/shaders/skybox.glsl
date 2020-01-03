#type vertex
#version 410 core

#import "basic_vertex"

void main() {
	o_vs_to_fs.position = a_position;

	gl_Position = u_transform.projection * mat4(mat3(u_transform.view)) * vec4(a_position, 1.0);
}

#type fragment
#version 410 core

#import "basic_fragment"

uniform samplerCube u_texture;

void main() {
	o_color = texture(u_texture, i_vs_to_fs.position);
}