#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2DArray textureArray;
//uniform sampler2D texture2;

uniform int textureIndex;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	FragColor = texture(textureArray, vec3(TexCoord.x, TexCoord.y, textureIndex));
	//FragColor = vec4(1, 1, 1, 1);
}
