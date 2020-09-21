#light_mode none

#type vertex
#version 410 core

#import standard_vertex_attributes
#import standard_vertex

void main() {
	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

#import standard_fragment

void main() {
	o_color = vec4(1, 0, 1, 1);
}