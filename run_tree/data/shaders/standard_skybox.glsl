#type vertex
#version 410 core

#import "standard_vertex"

out VS_TO_FS {
	vec3 position;
	vec3 normal;
	vec2 uv;
} o_v2f;

void main() {
	o_v2f.position = a_position;

	gl_Position = (u_transform.projection * mat4(mat3(u_transform.view)) * vec4(a_position, 1.0)).xyww;
}

#type fragment
#version 410 core

#import "standard_fragment"

in VS_TO_FS {
	vec3 position;
	vec3 normal;
	vec2 uv;
} i_v2f;

uniform samplerCube u_skybox;

void main() {
	o_color = texture(u_skybox, i_v2f.position);
}