out vec4 o_color;

in VS_TO_FS {
	vec3 position;
	vec3 normal;
	vec2 uv;
} i_v2f;

uniform struct Camera {
	vec3 position;
} u_camera;

uniform sampler2D u_texture;