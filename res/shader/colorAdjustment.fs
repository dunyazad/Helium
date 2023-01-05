#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform float controlValue;


// RGB <-> YUV
//Y = 0.299R + 0.587G + 0.114B
//U '= (B-Y) * 0.565
//V '= (R-Y) * 0.713

//R = Y + 1.403V '
//G = Y - 0.344U - 0.714V '
//B = Y + 1.770U '

vec3 RGB2YUV(vec3 rgb)
{
	float y = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
	float u = (rgb.b - y) * 0.565;
	float v = (rgb.r - y) * 0.713;
	return vec3(y, u, v);
}

vec3 YUV2RGB(vec3 yuv)
{
	float r = yuv.x + 1.403 * yuv.z;
	float g = yuv.x - 0.344 * yuv.y - 0.714 * yuv.z;
	float b = yuv.x + 1.770 * yuv.y;
	return vec3(r, g, b);
}

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	vec3 rgb = texture(texture1, TexCoord).xyz;
	vec3 yuv = RGB2YUV(rgb);
	yuv.x = yuv.x + controlValue;

	vec3 rgbOut = YUV2RGB(yuv);
	FragColor = vec4(rgbOut, 1);

	//FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(1, 1, 1, 1);
}
