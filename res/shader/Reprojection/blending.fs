#version 330

out vec4 FragColor;

in vec3 ourPosition;
in vec4 ourColor;
in vec3 ourNormal;
in vec2 TexCoord;

uniform float controlValues[256];

uniform sampler2D customDataSampler;
uniform sampler2DArray textureArray;

uniform int incremental;

uniform int screenWidth;
uniform int screenHeight;

uniform int imageWidth;
uniform int imageHeight;
uniform int frameCount;

uniform mat4 projection;
uniform mat4 view;

float FLT_EPSILON = 1.192092896e-06F;
float FLT_MAX = 3.402823466e+38F;
float PI = 3.14159265359;
float PI2 = 6.2831853071;

vec3 GetWorldPosition() {
	vec4 ndc = vec4((gl_FragCoord.x / float(screenWidth) - 0.5) * 2.0, (gl_FragCoord.y / float(screenHeight) - 0.5) * 2.0, (gl_FragCoord.z - 0.5) * 2.0, 1.0);

	mat4 inverse_view_proj = inverse(projection * view);

	// Convert NDC throuch inverse clip coordinates to view coordinates
	vec4 clip = inverse_view_proj * ndc;
	vec3 worldPosition = (clip / clip.w).xyz;
	//vec3 worldPosition = clip.xyz;

	return worldPosition;
}

float GetFX(int frameIndex) {
	int index = frameIndex * (1 + 3 + 3 + 16);
	float fx = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	return fx;
}

mat4 GetFrameInverseMatrix(int frameIndex) {
	int index = frameIndex * (1 + 3 + 3 + 16) + 7;
	mat4 m;
	for(int i = 0; i < 4; i++) {
		float e0 = texelFetch(customDataSampler, ivec2(index + i * 4, 0), 0).r;
		float e1 = texelFetch(customDataSampler, ivec2(index + i * 4 + 1, 0), 0).r;
		float e2 = texelFetch(customDataSampler, ivec2(index + i * 4 + 2, 0), 0).r;
		float e3 = texelFetch(customDataSampler, ivec2(index + i * 4 + 3, 0), 0).r;
		m[i] = vec4(e0, e1, e2, e3);
	}

	return m;
}

vec3 GetFramePosition(int frameIndex) {
	int index = frameIndex * (1 + 3 + 3 + 16) + 1;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return vec3(x, y, z);
}

vec3 GetFrameDirection(int frameIndex) {
	int index = frameIndex * (1 + 3 + 3 + 16) + 4;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return -vec3(x, y, z);
}

vec3 RayPlaneIntersection(vec3 planePoint, vec3 planeNormal, vec3 rayOrigin, vec3 rayDirection) {
	float ndotu = dot(planeNormal, rayDirection);

	//if (abs(ndotu) < FLT_EPSILON) {
	//	//cout << "No intersection or line is within plane." << endl;
	//}

	vec3 w = rayOrigin - planePoint;
	float si = -dot(planeNormal, w) / ndotu;
	return w + si * rayDirection + planePoint;
}

vec2 WorldToUV(int frameIndex, vec3 worldPosition) {
	vec4 ip4 = GetFrameInverseMatrix(frameIndex) * vec4(worldPosition, 1);
	vec3 planePoint = vec3(0, 0, GetFX(frameIndex));
	vec3 planeNormal = vec3(0, 0, 1);
	vec3 rayOrigin = vec3(0, 0, 0);
	vec3 rayDirection = normalize(vec3(ip4.x, ip4.y, ip4.z));
	vec3 intersection = RayPlaneIntersection(planePoint, planeNormal, rayOrigin, rayDirection);
	float u = ((intersection.x / (float(imageWidth) * 0.5)) * 0.5 + 0.5);
	float v = 1.0 - ((intersection.y / (float(imageHeight) * 0.5)) * 0.5 + 0.5);
	return vec2(u, v);
}

bool IsInsideOfFOV(vec2 uv) {
	return (0.0 <= uv.x) && (uv.x <= 1.0) && (0.0 <= uv.y) && (uv.y <= 1.0);
}

bool equals(float a, float b) {
	if(abs(a - b) < FLT_EPSILON) {
		return true;
	} else {
		return false;
	}
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

void main() {
	vec3 worldPosition = GetWorldPosition();

	vec4 accColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < frameCount; i++) {
		vec2 uv = WorldToUV(i, worldPosition);
		if(IsInsideOfFOV(uv)) {
			vec3 worldNormal = normalize(GetFramePosition(i) - worldPosition);
			vec3 frameDirection = GetFrameDirection(i);
			float angle = acos(dot(frameDirection, worldNormal));
			float frameDistance = distance(worldPosition, GetFramePosition(i));
			float ratio = pow(cos(angle), 2) / pow(frameDistance / 1000, 2);
			vec4 color = texture(textureArray, vec3(uv.x, uv.y, i));
			accColor = mix(accColor, color, ratio);
		}
	}
	FragColor = accColor;
}
