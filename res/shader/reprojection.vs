#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 ourPosition;
out vec3 ourNormal;
out vec4 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	ourPosition = (model * vec4(aPos.x, aPos.y, aPos.z, 1.0)).xyz;
	ourColor = aColor;
	ourNormal = normalize(aNormal);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
