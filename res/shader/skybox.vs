#version 330

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 ourColor;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
	TexCoords = aPos;
}
