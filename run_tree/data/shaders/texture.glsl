#type vertex
#version 410 core

#import "basic"

void main() {
	vs_out.position = obj_to_world_space(a_position);
	vs_out.normal = normal_to_world_space(a_normal);
	vs_out.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

out vec4 o_color;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 uv;
} fs_in;

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, fs_in.uv);
}