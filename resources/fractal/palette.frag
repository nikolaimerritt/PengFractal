#version 400 core

in vec2 tex_coord;
out vec4 frag_color;


uniform float height = 0.5625;
uniform vec2 complex_centre = vec2(0, 0);
uniform float zoom = 1;

//const vec3 palette[3] = vec3[3](
//	vec3(240, 140, 98) / 255,
//	vec3(185, 46, 88) / 255,
//	vec3(64, 39, 98) / 255
//);

const vec3 palette[6] = vec3[6](
	vec3(67,41,103) / 255,
	vec3(120,45,101) / 255,
	vec3(195,48,93) / 255,
	vec3(240,68,94) / 255,
	vec3(253,147,103) / 255,
	vec3(253,205,131) / 255
);

const float max_length = 2.0;
const uint max_iters = 1000;
const vec2 screen_centre = vec2(0.5, 0.5);
const vec4 background = vec4(0);

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
		frag_color = background;
	}
	else {
		int fst_colour = int(mod(iters, palette.length()));
		int snd_colour = int(mod(iters + 1, palette.length()));
		float blend = length(iterate) / max_length;
		vec3 colour = mix(palette[fst_colour], palette[snd_colour], blend);  
		frag_color = vec4(colour, 0);
	}
}
