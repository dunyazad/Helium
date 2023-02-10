#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec4 ourColor;

void main()
{
	ourColor = aColor;
	 gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
