#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2DArray textureArray;

uniform int textureIndex;

void main()
{
	float iu = floor(TexCoord.x);
	float u = TexCoord.x - floor(TexCoord.x);
	float iv = floor(TexCoord.y);
	float v = TexCoord.y - floor(TexCoord.y);
	FragColor = texture(textureArray, vec3(u, v, iu));
}
