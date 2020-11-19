#version 330

out vec4 FragColor;

in vec4 g_color;

void main(void)
{
    gl_FragColor = g_color;
};