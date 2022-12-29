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

// Taken from https://gamedev.stackexchange.com/questions/59797/glsl-shader-change-hue-saturation-brightness
vec3 hsv_to_rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

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
	if (divergence_scale - 1.0 < 0.00001) {
		frag_color = background;
	}
	else {
		vec3 hsv = vec3(divergence_scale, 1.0, 1.0);
		frag_color = vec4(hsv_to_rgb(hsv), 1.0);
	}
}
