#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 v_color;

void main()
{
	v_color = aColor;
	gl_Position = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
