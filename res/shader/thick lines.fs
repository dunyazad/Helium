#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 vLineCenter;

uniform float lineWidth;
uniform float blendFactor; //1.5..2.5

void main(void)
{
      vec4 col = ourColor;        
      float d = length(vLineCenter-gl_FragCoord.xy);
      float w = lineWidth;
      if (d>w)
        col.w = 0;
      else
        col.w *= pow(float((w-d)/w), blendFactor);
      gl_FragColor = col;
};