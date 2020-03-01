#type vertex
#version 410 core

#import standard_vertex_attributes
#import standard_vertex

out V2F {
	vec2 uv;
} o_v2f;

void main() {
	o_v2f.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

out vec4 o_color;

in V2F {
	vec2 uv;
} i_v2f;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
	o_color = u_color * texture(u_texture, i_v2f.uv);
}