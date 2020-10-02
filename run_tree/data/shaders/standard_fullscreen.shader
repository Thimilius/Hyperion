#light_mode none

#type vertex
#version 410 core

out V2F {
	vec2 uv;
} o_v2f;

void main() {
	vec2 vertices[3] = vec2[3](vec2(-1.0, -1.0f), vec2(-1.0, 3.0), vec2(3.0f, -1.0));
	vec4 position = vec4(vertices[gl_VertexID], 0.0, 1.0);

	o_v2f.uv = 0.5 * position.xy + vec2(0.5);

	gl_Position = position;
}

#type fragment
#version 410 core

out vec4 o_color;

in V2F {
	vec2 uv;
} i_v2f;

uniform sampler2D u_texture;

void main() {
	o_color = texture(u_texture, i_v2f.uv);
}