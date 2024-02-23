#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screen_texture;
uniform float time;

void main()
{
	FragColor = vec4(texture(screen_texture, TexCoord).xyz, 1.0);
}
