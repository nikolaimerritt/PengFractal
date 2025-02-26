#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_col;
layout(location = 2) in vec2 a_tex_coord;

out vec4 vertex_color;
out vec2 tex_coord;

uniform vec4 color = vec4(1, 1, 1, 1);
uniform mat4 transform = mat4(1);

void main()
{
    gl_Position = transform * vec4(a_pos, 1.0);
    vertex_color = vec4(a_col, 1) * color;
    tex_coord = a_tex_coord;
}