#type vertex
#version 330 core

layout(location = 0) in vec3 attrib_position;

uniform mat4 uniform_view_projection;
uniform mat4 uniform_transform;

out vec3 v_position;

void main()
{
	v_position = attrib_position;
	gl_Position = uniform_view_projection * uniform_transform * vec4(attrib_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_position;

uniform vec4 u_color;

void main() { color = u_color; }