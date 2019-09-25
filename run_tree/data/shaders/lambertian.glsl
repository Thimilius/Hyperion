#type vertex
#version 410 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 uv;
} vs_out;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec3 transform_normal(vec3 normal) {
	return normalize(mat3(transpose(inverse(u_transform.model))) * a_normal);
}

vec3 obj_to_world_space(vec3 position) {
	return (u_transform.model * vec4(position, 1.0)).xyz;
}

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	vs_out.position = obj_to_world_space(a_position);
	vs_out.normal =  transform_normal(a_normal);
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

uniform struct Light {
	vec3 position;
	vec3 color;
} u_light;

uniform sampler2D u_texture;

void main() {
	// Texture
	vec4 texture_color = texture(u_texture, fs_in.uv);
	
	// Ambient
	float ambient_intensity = 0.2;
	vec3 ambient_lighting = vec3(ambient_intensity, ambient_intensity, ambient_intensity);
	
	// Diffuse
	float diffuse_intensity = 0.8;
	vec3 light_direction = normalize(u_light.position - fs_in.position);
	diffuse_intensity = diffuse_intensity * max(dot(fs_in.normal, light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * u_light.color;
	
	// Final lighting
	vec3 lighting_color = ambient_lighting + diffuse_lighting;
	
	// Final color
	o_color = vec4(lighting_color, 1.0);
}