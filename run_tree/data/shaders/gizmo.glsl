#type vertex
#version 410 core

#import "basic"

void main() {
	vs_out.position = obj_to_world_space(a_position);
	vs_out.normal = normal_to_world_space(a_normal);
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

uniform struct Camera {
	vec3 position;
} u_camera;

uniform vec3 u_color;

void main() {
	// Ambient
	float ambient_intensity = 0.2;
	vec3 ambient_lighting = vec3(ambient_intensity, ambient_intensity, ambient_intensity);
	
	// Diffuse
	float diffuse_intensity = 0.8;
	vec3 to_light_direction = normalize(u_camera.position - fs_in.position);
	diffuse_intensity = diffuse_intensity * max(dot(fs_in.normal, to_light_direction), 0.0);
	vec3 diffuse_lighting = vec3(diffuse_intensity);
	
	// Final lighting
	vec3 lighting_color = ambient_lighting + diffuse_lighting;
	
	// Final color
	o_color = vec4(lighting_color * u_color, 1.0);
}