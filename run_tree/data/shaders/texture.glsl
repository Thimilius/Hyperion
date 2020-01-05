#type vertex
#version 410 core

#import "basic_vertex"

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);
	o_v2f.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

#import "basic_fragment"

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, i_v2f.uv);
}