#type vertex
#version 330 core
layout(location = 0) in vec3 attrib_position;
layout(location = 1) in vec2 attrib_tex_coords;

uniform mat4 uniform_view_projection;
uniform mat4 uniform_transform;

out vec2 vertex_tex_coords;

void main()
{
    vertex_tex_coords = attrib_tex_coords;
    gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 output_colour;

in vec2 vertex_tex_coords;

uniform sampler2D tex_sampler;

void main()
{
    output_colour = texture(tex_sampler, vertex_tex_coords * 10.0);
}