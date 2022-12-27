#version 330 core

in vec2 tex_coord;

out vec4 frag_color;

uniform vec4 background  = vec4(1);
uniform vec4 foreground = vec4(1);
uniform uint max_iters = 80u;
uniform float max_length = 2.0;

uniform sampler2D color_tex;

vec2 complex_multiply(vec2 fst, vec2 snd) {
	float x = fst.x * snd.x - fst.y * snd.y;
	float y = fst.x * snd.y + fst.y * snd.x;

	return vec2(x, y);
}

void main()
{
	vec2 iterate = tex_coord;
	uint iters = 0u;
	
	for (; iters < max_iters && length(iterate) <= max_length; iters++) {
		iterate = complex_multiply(iterate, iterate) + tex_coord;
	}

	// 0 is convergent, 1 is divergent
	float divergence_scale = length(iterate) / max_length;
	frag_color = mix(foreground, background, divergence_scale); 
}