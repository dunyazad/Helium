#version 330

out vec4 FragColor;

in vec3 ourPosition;
in vec4 ourColor;
in vec3 ourNormal;
in vec2 TexCoord;

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

#define LUMINANCE_PRESERVATION 0.75

#define EPSILON 1e-10

float saturate(float v) { return clamp(v, 0.0,       1.0);       }
vec2  saturate(vec2  v) { return clamp(v, vec2(0.0), vec2(1.0)); }
vec3  saturate(vec3  v) { return clamp(v, vec3(0.0), vec3(1.0)); }
vec4  saturate(vec4  v) { return clamp(v, vec4(0.0), vec4(1.0)); }

vec3 ColorTemperatureToRGB(float temperatureInKelvins)
{
	vec3 retColor;
	
    temperatureInKelvins = clamp(temperatureInKelvins, 1000.0, 40000.0) / 100.0;
    
    if (temperatureInKelvins <= 66.0)
    {
        retColor.r = 1.0;
        retColor.g = saturate(0.39008157876901960784 * log(temperatureInKelvins) - 0.63184144378862745098);
    }
    else
    {
    	float t = temperatureInKelvins - 60.0;
        retColor.r = saturate(1.29293618606274509804 * pow(t, -0.1332047592));
        retColor.g = saturate(1.12989086089529411765 * pow(t, -0.0755148492));
    }
    
    if (temperatureInKelvins >= 66.0)
        retColor.b = 1.0;
    else if(temperatureInKelvins <= 19.0)
        retColor.b = 0.0;
    else
        retColor.b = saturate(0.54320678911019607843 * log(temperatureInKelvins - 10.0) - 1.19625408914);

    return retColor;
}

float Luminance(vec3 color)
{
    float fmin = min(min(color.r, color.g), color.b);
	float fmax = max(max(color.r, color.g), color.b);
	return (fmax + fmin) / 2.0;
}

vec3 HUEtoRGB(float H)
{
    float R = abs(H * 6.0 - 3.0) - 1.0;
    float G = 2.0 - abs(H * 6.0 - 2.0);
    float B = 2.0 - abs(H * 6.0 - 4.0);
    return saturate(vec3(R,G,B));
}

vec3 HSLtoRGB(in vec3 HSL)
{
    vec3 RGB = HUEtoRGB(HSL.x);
    float C = (1.0 - abs(2.0 * HSL.z - 1.0)) * HSL.y;
    return (RGB - 0.5) * C + vec3(HSL.z);
}
 
vec3 RGBtoHCV(vec3 RGB)
{
    // Based on work by Sam Hocevar and Emil Persson
    vec4 P = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0, 2.0/3.0) : vec4(RGB.gb, 0.0, -1.0/3.0);
    vec4 Q = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
    float C = Q.x - min(Q.w, Q.y);
    float H = abs((Q.w - Q.y) / (6.0 * C + EPSILON) + Q.z);
    return vec3(H, C, Q.x);
}

vec3 RGBtoHSL(vec3 RGB)
{
    vec3 HCV = RGBtoHCV(RGB);
    float L = HCV.z - HCV.y * 0.5;
    float S = HCV.y / (1.0 - abs(L * 2.0 - 1.0) + EPSILON);
    return vec3(HCV.x, S, L);
}

vec3 GetWorldPosition()
{
	vec4 ndc = vec4(
		(gl_FragCoord.x / float(screenWidth) - 0.5) * 2.0,
		(gl_FragCoord.y / float(screenHeight) - 0.5) * 2.0,
		(gl_FragCoord.z - 0.5) * 2.0,
		1.0);

	mat4 inverse_view_proj = inverse(projection * view);

	// Convert NDC throuch inverse clip coordinates to view coordinates
	vec4 clip = inverse_view_proj * ndc;
	vec3 worldPosition = (clip / clip.w).xyz;
	//vec3 worldPosition = clip.xyz;

	return worldPosition;
}

float GetFX(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16 + 9 + 16);
	float fx = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	return fx;
}

mat4 GetFrameInverseMatrix(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16 + 9 + 16) + 7;
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

vec3 GetFramePosition(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16 + 9 + 16) + 1;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return vec3(x, y, z);
}

vec3 GetFrameDirection(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16 + 9 + 16) + 4;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return -vec3(x, y, z);
}

vec3 RayPlaneIntersection(vec3 planePoint, vec3 planeNormal, vec3 rayOrigin, vec3 rayDirection)
{
	float ndotu = dot(planeNormal, rayDirection);

	//if (abs(ndotu) < FLT_EPSILON) {
	//	//cout << "No intersection or line is within plane." << endl;
	//}

	vec3 w = rayOrigin - planePoint;
	float si = -dot(planeNormal, w) / ndotu;
	return w + si * rayDirection + planePoint;
}

vec2 WorldToUV(int frameIndex, vec3 worldPosition)
{
	vec4 ip4 = GetFrameInverseMatrix(frameIndex) * vec4(worldPosition, 1);
	vec3 planePoint = vec3(0, 0, GetFX(frameIndex));
	vec3 planeNormal = vec3(0, 0, 1);
	vec3 rayOrigin = vec3(0, 0, 0);
	vec3 rayDirection = normalize(vec3(ip4.x, ip4.y, ip4.z));
	vec3 intersection = RayPlaneIntersection(planePoint, planeNormal, rayOrigin, rayDirection);
	float u = ((intersection.x / ((float(imageWidth) / 7.5) * 0.5)) * 0.5 + 0.5);
	float v = 1.0 - ((intersection.y / ((float(imageHeight) / 7.5) * 0.5)) * 0.5 + 0.5);
	return vec2(u, v);
}

bool IsInsideOfFOV(vec2 uv)
{
	return (0.0 <= uv.x) && (uv.x <= 1.0) && (0.0 <= uv.y) && (uv.y <= 1.0);
}

bool equals(float a, float b) {
	if(abs(a - b) < FLT_EPSILON) {
		return true;
	} else {
		return false;
	}
}

// vec2 GetRadialDistortion(vec2 xy, vec2 xyCenter)
// {
// 	float RadialDistortionFactors[6];
// 	for (int i = 0; i < 6; i++)
// 	{
// 		RadialDistortionFactors[i] = 0;
// 	}

// 	float dx = xy.x - xyCenter.x;
// 	float dy = xy.y - xyCenter.y;

// 	float rK1 = distance(xy, xyCenter);
// 	float rK2 = rK1 * rK1;
// 	float rK3 = rK2 * rK2;
// 	float rK4 = rK3 * rK3;
// 	float rK5 = rK4 * rK4;
// 	float rK6 = rK5 * rK5;

// 	float lastFactor = distance(xy, xyCenter);
// 	float factor = 1.0f;
// 	for (int i = 0; i < 6; i++)
// 	{
// 		factor = factor + RadialDistortionFactors[i] * lastFactor * lastFactor;
// 		lastFactor = lastFactor * lastFactor;
// 	}

// 	return vec2(xyCenter.x + dx / factor, xyCenter.y + dy / factor);
// }

void main()
{
	vec3 worldPosition = GetWorldPosition();

	int validFrame = 0;

	float frameDistanceDistance = FLT_MAX;
	int frameDistanceFrame = 0;
	vec2 frameDistanceFrameUV;

	float uvCenterDistance = FLT_MAX;
	int uvCenterFrame = 0;
	vec2 uvCenterFrameUV;
	
	float nearStraightAngle = -FLT_MAX;
	float nearStraightFrame = 0;
	vec2 nearStraightFrameUV;

	float uvDistanceMax = distance(vec2(0.5, 0.5), vec2(1, 1));
	
	for(int i = 0; i < frameCount; i++)
	{
		vec2 uv = WorldToUV(i, worldPosition);
		if (IsInsideOfFOV(uv))
		{
			float frameDistance = distance(worldPosition, GetFramePosition(i));
			if(frameDistance < frameDistanceDistance)
			{
				frameDistanceDistance = frameDistance;
				frameDistanceFrame = i;
				frameDistanceFrameUV = uv;
			}

			float uvDistance = distance(uv, vec2(0.5, 0.5));
			if(uvDistance < 0.0000001) uvDistance = 0.0000001;
			if(uvDistance < uvCenterDistance)
			{
				uvCenterDistance = uvDistance;
				uvCenterFrame = i;
				uvCenterFrameUV = uv;
			}

			vec3 frameDirection = GetFrameDirection(i);
			float angle = acos(dot(frameDirection, ourNormal));
			if(angle > nearStraightAngle)
			{
				nearStraightAngle = angle;
				nearStraightFrame = i;
				nearStraightFrameUV = uv;
			}

			validFrame++;
		}
	}

	vec4 fdt = texture(textureArray, vec3(frameDistanceFrameUV.x, frameDistanceFrameUV.y, frameDistanceFrame));
	// vec4 accumulatedColorFDT = fdt;

	vec4 uvct = texture(textureArray, vec3(uvCenterFrameUV.x, uvCenterFrameUV.y, uvCenterFrame));
	vec4 accumulatedColorUV = fdt;

	// vec4 nst = texture(textureArray, vec3(nearStraightFrameUV.x, nearStraightFrameUV.y, nearStraightFrame));
	// vec4 accumulatedColorAngle = nst;

	for(int i = 0; i < frameCount; i++)
	{
		vec2 uv = WorldToUV(i, worldPosition);
		vec3 frameDirection = GetFrameDirection(i);
		float angle = acos(dot(frameDirection, ourNormal));

		if (IsInsideOfFOV(uv) && (abs(angle) > PI / 2))
		{
			vec4 color = texture(textureArray, vec3(uv.x, uv.y, i));

			//float frameDistance = distance(worldPosition, GetFramePosition(i));
			float ratioFrameDistance = (i == frameDistanceFrame ? 1 : 0);

			float uvDistance = distance(uv, vec2(0.5, 0.5));
			if(uvDistance < 0.0000001) uvDistance = 0.0000001;

			//float ratioUV = 1 - pow(abs(sin(PI * (uvDistance / uvDistanceMax) / 2.0)), 3.0);
			float ratioUV = 1 - uvDistance / uvDistanceMax;
			if(i != uvCenterFrame)
			{
				ratioUV = ratioUV * 0.2;
			}

			// float ratioAngle = 1 - pow(abs(sin(PI * (angle / PI) / 2.0)), 3.0);
			float ratioAngle = clamp(abs(angle) / PI, 0, 1);

			// accumulatedColorFDT = mix(accumulatedColorFDT, color, ratioFrameDistance);
			accumulatedColorUV = mix(accumulatedColorUV, color, ratioUV);
			// accumulatedColorAngle = mix(accumulatedColorAngle, color, ratioAngle);
		}
	}

	// vec4 resultColor = mix(accumulatedColorUV, accumulatedColorAngle, 0.0);
	// resultColor = mix(resultColor, accumulatedColorFDT, 0.0);

	// FragColor = resultColor;

	//FragColor = mix(fdt, accumulatedColorUV, uvCenterDistance / uvDistanceMax);
	// FragColor = mix(fdt, accumulatedColorUV, 1);
	FragColor = accumulatedColorUV;

	//FragColor = vec4(ratioAngle, 0, 0, 1);
}
