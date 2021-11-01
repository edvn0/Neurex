#type vertex
#version 410 core

layout(location = 0) in vec4 a_Color;
layout(location = 1) in vec3 a_Position;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TextureIndex;
layout(location = 4) in float a_Tiling;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TextureIndex;
out float v_Tiling;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TextureIndex = a_TextureIndex;
	v_Tiling = a_Tiling;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TextureIndex;
in float v_Tiling;

uniform sampler2D u_Textures[16];

void main() { color = texture(u_Textures[int(v_TextureIndex)], v_TexCoord * v_Tiling) * v_Color; }