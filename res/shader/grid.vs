#version 330
#extension GL_KHR_vulkan_glsl : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 ourColor;
out vec3 nearPoint;
out vec3 farPoint;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

vec3 gridPlane[4] = vec3[] (vec3(-1, -1, 0), vec3(1, -1, 0), vec3(-1, 1, 0), vec3(1, 1, 0));

void main()
{
    vec3 p = gridPlane[gl_VertexID].xyz;

	ourColor = aColor;
    
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane

	gl_Position = vec4(p, 1.0);
}
