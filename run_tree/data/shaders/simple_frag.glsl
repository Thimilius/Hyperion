#version 410 core

out vec4 o_color;

in VS_OUT {
	vec2 uv;
	vec4 color;
} fs_in;

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, fs_in.uv);
}