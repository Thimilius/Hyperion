#type vertex
#version 410 core

#import "basic_vertex"

void main() {
	o_v2f.position = obj_to_world_space(a_position);
	o_v2f.normal = normal_to_world_space(a_normal);
	o_v2f.uv = a_uv;

	gl_Position = obj_to_clip_space(a_position);
}

#type fragment
#version 410 core

#import "basic_fragment"

uniform struct Light {
	vec3 position;
	vec4 color;
} u_light;

uniform sampler2D u_texture;

void main() {
	// Texture
	vec4 texture_color = texture(u_texture, i_v2f.uv);
	
	// Ambient
	float ambient_intensity = 0.2;
	vec3 ambient_lighting = vec3(ambient_intensity, ambient_intensity, ambient_intensity);
	
	// Diffuse
	float diffuse_intensity = 0.8;
	vec3 to_light_direction = normalize(u_light.position - i_v2f.position);
	diffuse_intensity = diffuse_intensity * max(dot(i_v2f.normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = diffuse_intensity * u_light.color.xyz;
	
	// Specular
	float specular_intensity = 0.5;
	float specular_exponent = 25;
	vec3 to_camera_direction = normalize(u_camera.position - i_v2f.position);
	vec3 reflection_direction = reflect(-to_light_direction, i_v2f.normal);
	specular_intensity = specular_intensity * pow(max(dot(to_camera_direction, reflection_direction), 0.0), specular_exponent);
	vec3 specular_lighting = specular_intensity * u_light.color.xyz;
	
	// Final lighting
	vec3 lighting_color = ambient_lighting + diffuse_lighting + specular_lighting;
	
	// Final color
	o_color = vec4(lighting_color, 1.0) * texture_color;
}