#version 400 core

in vec2 tex_coord;
out vec4 frag_color;

uniform vec4 background  = vec4(0);
uniform uint max_iters = 600;
uniform float max_length = 2.0;

uniform float height = 0.5625;
uniform vec2 complex_centre = vec2(0, 0);
uniform float zoom = 1;
uniform vec2 screen_centre = vec2(0.5, 0.5);

const vec3 palette[3] = vec3[3](
	vec3(240, 140, 98) / 255,
	vec3(185, 46, 88) / 255,
	vec3(64, 39, 98) / 255
);

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
	uint iters = 0;
	
	for (; iters < max_iters && length(iterate) <= max_length; iters++) {
		iterate = complex_multiply(iterate, iterate) + complex_point;
	}

	if (iters - max_iters <= 0.0001) {
		frag_color = vec4(0);
	}
	else {
		const float log_2 = log(2);
		float log_complex_point = log(dot(complex_point, complex_point)) / 2;
		float blend = clamp(1 - log(log_complex_point / log_2) / log_2, 0, 1);

		int bg_index = int(mod(iters, palette.length()));
		int fg_index = int(mod(iters + 1, palette.length()));
		vec3 colour = mix(palette[bg_index], palette[fg_index], blend);  
		frag_color = vec4(colour, 1);
	}
	
}
