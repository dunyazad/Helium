#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2DArray textureArray;

uniform int textureIndex;

void main()
{
	FragColor = texture(textureArray, vec3(TexCoord.x, TexCoord.y, textureIndex));
}
