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
	int index = frameIndex * (1 + 3 + 3 + 16);
	float fx = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	return fx;
}

mat4 GetFrameInverseMatrix(int frameIndex)
{
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

vec3 GetFramePosition(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16) + 1;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return vec3(x, y, z);
}

vec3 GetFrameDirection(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 3 + 16) + 4;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return vec3(x, y, z);
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

void main()
{
	vec3 worldPosition = GetWorldPosition();

	vec4 acculumatedColor = vec4(0, 0, 0, 0);
	
	int validFrame = 0;
	for(int i = 0; i < frameCount; i++)
	{
		vec2 uv = WorldToUV(i, worldPosition);
		if (IsInsideOfFOV(uv))
		{
			validFrame++;
		}
	}

	float uvCenterDistance = FLT_MAX;
	int uvCenterFrame = 0;
	for(int i = 0; i < frameCount; i++)
	{
		vec2 uv = WorldToUV(i, worldPosition);
		if (IsInsideOfFOV(uv))
		{
			vec3 frameDirection = GetFrameDirection(i);
			float fdDotNPlusOne = dot(frameDirection, ourNormal) + 1.0;
			float ratio = 1.0 / (1.0 - (fdDotNPlusOne * 0.5));
			ratio = clamp(ratio, 0.0, 1.0);

			// vec3 framePosition = GetFramePosition(i);
			// float dist = distance(worldPosition, framePosition);
		
			float dist = distance(uv, vec2(0.5, 0.5));
			if(dist < 0.0000001) dist = 0.0000001;
			if (dist < uvCenterDistance)
			{
				uvCenterDistance = dist;
				uvCenterFrame = i;
			}

			// //float weight = 1 - (dist / sqrt(0.5));
			// float weight = 2.0 * (dist * dist) + 1.0;
			// weight = weight * ratio;
			// weight = clamp(weight, 0.0, 1.0);

			// vec4 tc = texture(textureArray, vec3(uv.x, uv.y, i));
			// tc = tc * weight;
			// acculumatedColor = acculumatedColor + tc / validFrame;
		}
	}

	//acculumatedColor.xyz = acculumatedColor.xyz;
	vec2 uv = WorldToUV(uvCenterFrame, worldPosition);
	vec4 tc = texture(textureArray, vec3(uv.x, uv.y, uvCenterFrame));
	FragColor = tc;
}
