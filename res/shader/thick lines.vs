#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;

uniform vec2 viewPort;

out vec4 ourColor;
out vec2 vLineCenter;

void main()
{
	ourColor = aColor;
	vec4 pp = projection * view * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = pp;
	vec2 vp = viewPort;
	vLineCenter = 0.5*(pp.xy + vec2(1, 1))*vp;
}
