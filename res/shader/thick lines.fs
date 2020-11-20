#version 330

out vec4 FragColor;

in vec4 v_color;

void main(void)
{
    gl_FragColor = v_color;
};