#light_mode forward

#type vertex
#version 410 core

#import standard_vertex_attributes
#import standard_vertex

out V2F {
	vec3 position;
	vec3 normal;
	vec2 uv;
} o_v2f;

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);
	o_v2f.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

#import standard_fragment
#import standard_lighting

in V2F {
	vec3 position;
	vec3 normal;
	vec2 uv;
} i_v2f;

uniform sampler2D u_texture;

void main() {
	// Texture
	vec4 texture_color = texture(u_texture, i_v2f.uv);
	
	// Final color
	o_color = vec4(calculate_full_phong_lighting(i_v2f.position, i_v2f.normal), 1.0) * texture_color;
}