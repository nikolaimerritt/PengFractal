#version 330 core

in vec2 tex_coord;
out vec4 frag_color;

uniform vec4 background  = vec4(1);
uniform vec4 foreground = vec4(1);
uniform uint max_iters = 500u;
uniform float max_length = 2.0;

uniform float height = 1.0;
uniform vec2 complex_centre = vec2(0.0, 0.0);
uniform float zoom = 1.0;
uniform vec2 screen_centre = vec2(0.5, 0.5);

vec2 complex_multiply(vec2 fst, vec2 snd) {
	float x = fst.x * snd.x - fst.y * snd.y;
	float y = fst.x * snd.y + fst.y * snd.x;

	return vec2(x, y);
}

void main()
{
	vec2 complex_point = (tex_coord - screen_centre) * max_length / zoom + complex_centre;
	complex_point = vec2(complex_point.x / height, complex_point.y);

	vec2 iterate = complex_point;
	uint iters = 0u;
	
	for (; iters < max_iters && length(iterate) <= max_length; iters++) {
		iterate = complex_multiply(iterate, iterate) + complex_point;
	}

	// 0 is convergent, 1 is divergent
	float divergence_scale = length(iterate) / max_length;
	frag_color = mix(foreground, background, divergence_scale); 
}