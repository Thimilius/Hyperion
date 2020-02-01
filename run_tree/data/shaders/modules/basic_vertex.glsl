layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;
	
out VS_TO_FS {
	vec3 position;
	vec3 normal;
	vec2 uv;
} o_v2f;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

uniform struct Camera {
	vec3 position;
} u_camera;

vec3 obj_to_world_space(vec3 position) {
	return (u_transform.model * vec4(position, 1.0)).xyz;
}

vec3 normal_to_world_space(vec3 normal) {
	return normalize(mat3(transpose(inverse(u_transform.model))) * a_normal);
}

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}