#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in vec4 vColor[];

out vec4 gColor;

void main() {
    vec3 position = gl_in[0].gl_Position.xyz;

    // Front face
    gColor = vColor[0];
    gl_Position = vec4(position + vec3(-1, -1,  1), 1.0);
    EmitVertex();
    gColor = vColor[0];
    gl_Position = vec4(position + vec3( 1, -1,  1), 1.0);
    EmitVertex();
    gColor = vColor[0];
    gl_Position = vec4(position + vec3(-1,  1,  1), 1.0);
    EmitVertex();
    gColor = vColor[0];
    gl_Position = vec4(position + vec3( 1,  1,  1), 1.0);
    EmitVertex();
    EndPrimitive();

    // Back face
    gColor = vColor[1];
    gl_Position = vec4(position + vec3(-1, -1, -1), 1.0);
    EmitVertex();
    gColor = vColor[1];
    gl_Position = vec4(position + vec3(-1,  1, -1), 1.0);
    EmitVertex();
    gColor = vColor[1];
    gl_Position = vec4(position + vec3( 1, -1, -1), 1.0);
    EmitVertex();
    gColor = vColor[1];
    gl_Position = vec4(position + vec3( 1,  1, -1), 1.0);
    EmitVertex();
    EndPrimitive();

    // Left face
    gColor = vColor[2];
    gl_Position = vec4(position + vec3(-1, -1,  1), 1.0);
    EmitVertex();
    gColor = vColor[2];
    gl_Position = vec4(position + vec3(-1,  1,  1), 1.0);
    EmitVertex();
    gColor = vColor[2];
    gl_Position = vec4(position + vec3(-1, -1, -1), 1.0);
    EmitVertex();
    gColor = vColor[2];
    gl_Position = vec4(position + vec3(-1,  1, -1), 1.0);
    EmitVertex();
    EndPrimitive();
}
