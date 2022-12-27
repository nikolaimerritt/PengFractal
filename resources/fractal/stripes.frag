#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 left_colour = vec4(1);
uniform vec4 right_colour = vec4(1);

uniform sampler2D color_tex;

void main()
{
	if (tex_coord.x < 0.5) {
		frag_color = left_colour;
	}
	else {
		frag_color = right_colour;
	}
}