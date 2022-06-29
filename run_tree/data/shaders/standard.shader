#property m_color Color
#property m_texture Texture

#render_order opaque

#type vertex
#version 450 core
	
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 4) in vec2 a_texture0;

out V2F {
	vec3 position;
	vec3 normal;
	vec2 texture0;
	vec4 light_space_position;
} o_v2f;

layout(std140, binding = 0) uniform Camera {
	mat4 view;
	mat4 projection;
} u_camera;

uniform mat4 u_model;
uniform mat4 u_light_space;

vec3 obj_to_world_space(vec3 position) {
	return (u_model * vec4(position, 1.0)).xyz;
}

vec3 normal_to_world_space(vec3 normal) {
	return normalize(mat3(transpose(inverse(u_model))) * a_normal);
}

vec4 obj_to_clip_space(vec3 position) {
	return u_camera.projection * u_camera.view * u_model * vec4(position, 1.0);
}

vec4 obj_to_light_space(vec3 position) {
	return u_light_space * u_model * vec4(position, 1.0);
}

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);
	o_v2f.texture0 = a_texture0;
	o_v2f.light_space_position = obj_to_light_space(a_position); 

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;

in V2F {
	vec3 position;
	vec3 normal;
	vec2 texture0;
	vec4 light_space_position;
} i_v2f;

struct Light {
	vec4 color;
	vec3 direction;
	float intensity;
	vec3 position;
	float range;
	float spot_inner_radius;
	float spot_outer_radius;
};

layout(std140, binding = 1) uniform Lighting {
	vec4 ambient_color;

	Light main_light;
	
	Light point_lights[128];
} u_lighting;
uniform uint u_light_count;
uniform uvec4 u_light_indices;
uniform sampler2D u_shadow_map;

uniform vec4 m_color;
uniform sampler2D m_texture;

float calculate_light_attenuation(float distance, float light_range) {
	float attenuation_linear = 4.5 / light_range;
	float attenuation_quadratic = 75.0 / (light_range * light_range);
	return 1.0 / (1.0 + attenuation_linear * distance + attenuation_quadratic * distance * distance);
}

vec3 calculate_phong_directional_light(vec3 position, vec3 normal, Light light) {
	// Diffuse
	float diffuse_intensity = light.intensity;
	vec3 to_light_direction = normalize(-light.direction);
	diffuse_intensity = diffuse_intensity * max(dot(normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * light.color.rgb;
	
	// Final lighting
	vec3 lighting_color = diffuse_lighting;

	return lighting_color;
}

vec3 calculate_phong_point_light(vec3 position, vec3 normal, Light light) {
	// Diffuse
	float diffuse_intensity = light.intensity;
	vec3 to_light_direction = normalize(light.position - position);
	diffuse_intensity = diffuse_intensity * max(dot(normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * light.color.rgb;
	
	// Attenuation
	float attenuation = calculate_light_attenuation(distance(light.position, position), light.range);
	
	// Final lighting
	vec3 lighting_color = attenuation * diffuse_lighting;

	return lighting_color;
}

float calculate_shadow(vec3 normal, vec4 light_space_position) {
	vec3 projection = light_space_position.xyz / light_space_position.w;
	projection = projection * 0.5 + 0.5;
	
	float closest_depth = texture(u_shadow_map, projection.xy).r;
	float current_depth = projection.z;
	
	float shadow_bias = max(0.005 * (1.0 - dot(normal, u_lighting.main_light.direction)), 0.005);
	float shadow = current_depth - shadow_bias > closest_depth ? 1.0 : 0.0;
	
	if (projection.z > 1.0) {
		shadow = 0.0;
	}
	
	return shadow;
}

vec4 calculate_phong_lighting(vec3 position, vec3 normal, vec4 light_space_position) {
	vec3 main_lighting = calculate_phong_directional_light(position, normal, u_lighting.main_light);

	vec3 point_lighting;
	for (int i = 0; i < u_light_count; i++) {
		point_lighting += calculate_phong_point_light(position, normal, u_lighting.point_lights[u_light_indices[i]]);
	}

	float shadow = calculate_shadow(normal, light_space_position);
	
	return vec4(u_lighting.ambient_color.rgb + (1.0 - shadow) * (main_lighting + point_lighting), 1.0);
}

void main() {
	vec4 texture_color = texture(m_texture, i_v2f.texture0);
	vec4 lighting_color = calculate_phong_lighting(i_v2f.position, i_v2f.normal, i_v2f.light_space_position);
	
	o_color = texture_color * lighting_color * m_color;
}