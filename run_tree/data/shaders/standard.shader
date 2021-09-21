#light_mode forward

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;

out V2F {
	vec3 position;
	vec3 normal;
} o_v2f;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

vec3 obj_to_world_space(vec3 position) {
	return (u_model * vec4(position, 1.0)).xyz;
}

vec3 normal_to_world_space(vec3 normal) {
	return normalize(mat3(transpose(inverse(u_model))) * a_normal);
}

vec4 obj_to_clip_space(vec3 position) {
	return u_projection * u_view * u_model * vec4(position, 1.0);
}

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in V2F {
	vec3 position;
	vec3 normal;
} i_v2f;

struct Light {
	float intensity;
	vec4 color;
	
	vec3 direction;
	
	vec3 position;
	float range;
	
	float spot_inner_radius;
	float spot_outer_radius;
};

struct Lighting {
	vec3 ambient_color;

	Light main_light;
};

uniform vec4 u_color;
uniform Lighting u_lighting;

vec3 calculate_phong_directional_light(vec3 position, vec3 normal, Light light) {
	float diffuse_intensity = light.intensity;
	vec3 to_light_direction = normalize(-light.direction);
	diffuse_intensity = diffuse_intensity * max(dot(normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * light.color.rgb;
	
	vec3 lighting_color = diffuse_lighting;

	return lighting_color;
}

vec4 calculate_phong_lighting(vec3 position, vec3 normal) {
	vec3 main_lighting = calculate_phong_directional_light(position, normal, u_lighting.main_light);

	return vec4(main_lighting + u_lighting.ambient_color, 1.0);
}

void main() {
	vec4 lighting_color = calculate_phong_lighting(i_v2f.position, i_v2f.normal);
	
	o_color = lighting_color * u_color;
}