#version 400 core

in vec2 tex_coord;
out vec4 frag_color;

uniform vec4 background  = vec4(1);
uniform vec4 foreground = vec4(1);
uniform uint max_iters = 80u;
uniform double max_length = 2.0;

uniform double height = 0.66666;
uniform dvec2 complex_centre = dvec2(0.167653780957681, 0);
uniform double zoom = 8977.51326;
uniform dvec2 screen_centre = dvec2(0.5, 0.5);

dvec2 complex_multiply(dvec2 fst, dvec2 snd) {
	double x = fst.x * snd.x - fst.y * snd.y;
	double y = fst.x * snd.y + fst.y * snd.x;

	return dvec2(x, y);
}

void main()
{
	dvec2 tex_coord_double = dvec2(tex_coord);
	dvec2 complex_point = (tex_coord_double - screen_centre) * max_length / zoom + complex_centre;
	complex_point = dvec2(complex_point.x / height, complex_point.y);

	dvec2 iterate = complex_point;
	uint iters = 0u;
	
	for (; iters < max_iters && length(iterate) <= max_length; iters++) {
		iterate = complex_multiply(iterate, iterate) + complex_point;
	}

	// 0 is convergent, 1 is divergent
	float divergence_scale = float(length(iterate) / max_length);
	frag_color = mix(foreground, background, divergence_scale); 
}