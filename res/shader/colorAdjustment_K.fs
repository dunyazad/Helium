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

#define WithQuickAndDirtyLuminancePreservation        

const float LuminancePreservationFactor = 1.0;

// Valid from 1000 to 40000 K (and additionally 0 for pure full white)
vec3 colorTemperatureToRGB(float temperature){
  // Values from: http://blenderartists.org/forum/showthread.php?270332-OSL-Goodness&p=2268693&viewfull=1#post2268693   
  mat3 m = (temperature <= 6500.0) ? mat3(vec3(0.0, -2902.1955373783176, -8257.7997278925690),
	                                      vec3(0.0, 1669.5803561666639, 2575.2827530017594),
	                                      vec3(1.0, 1.3302673723350029, 1.8993753891711275)) : 
	 								 mat3(vec3(1745.0425298314172, 1216.6168361476490, -8257.7997278925690),
   	                                      vec3(-2666.3474220535695, -2173.1012343082230, 2575.2827530017594),
	                                      vec3(0.55995389139931482, 0.70381203140554553, 1.8993753891711275)); 
  return mix(clamp(vec3(m[0] / (vec3(clamp(temperature, 1000.0, 40000.0)) + m[1]) + m[2]), vec3(0.0), vec3(1.0)), vec3(1.0), smoothstep(1000.0, 0.0, temperature));
}

void main()
{
	vec3 rgb = texture(texture1, TexCoord).xyz;
	FragColor = vec4(mix(rgb, colorTemperatureToRGB(4500 * controlValue), 0.15), 1);
}
