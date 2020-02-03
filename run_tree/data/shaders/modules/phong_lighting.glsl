struct Light {
	vec3 position;
	vec3 direction;
	
	vec4 color;
	float intensity;
	
	float radius;
	
	float spot_inner_radius;
	float spot_outer_radius;
};
const vec3 LIGHT_AMBIENT = vec3(0.1, 0.1, 0.1);
const float LIGHT_SPECULAR_EXPONENT = 15;

uniform Light u_light;

float calculate_attenuation(float distance, float light_range) {
	float attenuation_linear = 4.5 / light_range;
	float attenuation_quadratic = 75.0 / (light_range * light_range);
	return 1.0 / (1.0 + attenuation_linear * distance + attenuation_quadratic * distance * distance);
}

vec3 calculate_phong_point_light(Light light) {
	// Diffuse
	float diffuse_intensity = u_light.intensity;
	vec3 to_light_direction = normalize(u_light.position - i_v2f.position);
	diffuse_intensity = diffuse_intensity * max(dot(i_v2f.normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * u_light.color.rgb;
	
	// Specular
	float specular_intensity = u_light.intensity;
	vec3 to_camera_direction = normalize(u_camera.position - i_v2f.position);
	vec3 reflection_direction = reflect(-to_light_direction, i_v2f.normal);
	specular_intensity = specular_intensity * pow(max(dot(to_camera_direction, reflection_direction), 0.0), LIGHT_SPECULAR_EXPONENT);
	vec3 specular_lighting = specular_intensity * u_light.color.rgb;
	
	// Attenuation
	float attenuation = calculate_attenuation(distance(u_light.position, i_v2f.position), u_light.radius);
	
	// Final lighting
	vec3 lighting_color = attenuation * diffuse_lighting + attenuation * specular_lighting;

	return lighting_color;
}

vec3 calculate_full_phong_lighting() {
	return calculate_phong_point_light(u_light) + LIGHT_AMBIENT;
}