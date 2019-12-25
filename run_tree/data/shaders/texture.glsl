#type vertex
#version 410 core

#import "basic_vertex"

void main() {
	o_vs_to_fs.position = obj_to_world_space(a_position);
	o_vs_to_fs.normal = normal_to_world_space(a_normal);
	o_vs_to_fs.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

#import "basic_fragment"

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, i_vs_to_fs.uv);
}