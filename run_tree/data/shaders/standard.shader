#light_mode forward

#type vertex
#version 410 core
	
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 4) in vec2 a_texture0;

out V2F {
	vec3 position;
	vec3 normal;
	vec2 texture0;
} o_v2f;

uniform struct Transform {
	mat4 model;
	mat4 view;
	mat4 projection;
} u_transform;

vec3 obj_to_world_space(vec3 position) {
	return (u_transform.model * vec4(position, 1.0)).xyz;
}

vec3 normal_to_world_space(vec3 normal) {
	return normalize(mat3(transpose(inverse(u_transform.model))) * a_normal);
}

vec4 obj_to_clip_space(vec3 position) {
	return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
}

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);
	o_v2f.texture0 = a_texture0;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_color;
#ifdef HYP_EDITOR
layout(location = 1) out uint o_entity_id;
#endif

in V2F {
	vec3 position;
	vec3 normal;
	vec2 texture0;
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
uniform sampler2D u_texture;
uniform Lighting u_lighting;

#ifdef HYP_EDITOR
uniform uint u_entity_id;
#endif

vec3 calculate_phong_directional_light(vec3 position, vec3 normal, Light light) {
	// Diffuse
	float diffuse_intensity = light.intensity;
	vec3 to_light_direction = normalize(-vec3(0, -1, 0));	
	diffuse_intensity = diffuse_intensity * max(dot(normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * light.color.rgb;
	
	// Final lighting
	vec3 lighting_color = diffuse_lighting;

	return lighting_color;
}

vec4 calculate_phong_lighting(vec3 position, vec3 normal) {
	vec3 main_lighting = calculate_phong_directional_light(position, normal, u_lighting.main_light);

	return vec4(main_lighting + u_lighting.ambient_color, 1.0);
}

void main() {
	vec4 lighting_color = calculate_phong_lighting(i_v2f.position, i_v2f.normal);
	vec4 texture_color = texture(u_texture, i_v2f.texture0);
	
	o_color = lighting_color * u_color * texture_color;
#ifdef HYP_EDITOR
	o_entity_id = u_entity_id;
#endif
}