#light_mode forward

#type vertex
#version 410 core
	
layout(location = 0) in vec3 a_position;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_color;
#ifdef HYP_EDITOR
layout(location = 1) out uint o_entity_id;
#endif

uniform vec4 u_color;

#ifdef HYP_EDITOR
uniform uint u_entity_id;
#endif

void main() {
	o_color = u_color;
#ifdef HYP_EDITOR
	o_entity_id = u_entity_id;
#endif
}