#version 330

out vec4 FragColor;

in vec3 ourPosition;
in vec4 ourColor;
in vec2 TexCoord;

uniform sampler2DArray textureArray;
uniform sampler2D customDataSampler;

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
	int index = frameIndex * (1 + 3 + 9 + 16);
	float fx = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	return fx;
}

mat4 GetFrameInverseMatrix(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 9 + 16) + 13;
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
	int index = frameIndex * (1 + 3 + 9 + 16) + 1;
	float x = texelFetch(customDataSampler, ivec2(index, 0), 0).r;
	float y = texelFetch(customDataSampler, ivec2(index + 1, 0), 0).r;
	float z = texelFetch(customDataSampler, ivec2(index + 2, 0), 0).r;
	return vec3(x, y, z);
}

vec3 GetFrameDirection(int frameIndex)
{
	int index = frameIndex * (1 + 3 + 9 + 16) + 10;
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

	//vec3 worldPosition = ourPosition;

	//float minimumDistance = 3.402823466e+38F;
	//int minimumDistanceFrame = 0;

	int nearUVCenterFrame = 0;
	float minimumUVCenterDistance = 3.402823466e+38F;

	for(int i = 0; i < frameCount; i++)
	{
		//vec3 framePosition = GetFramePosition(i);
		//float dist = distance(worldPosition, framePosition);
		//if (dist < minimumDistance)
		//{
		//	minimumDistance = dist;
		//	minimumDistanceFrame = i;
		//}

		vec2 uv = WorldToUV(i, worldPosition);
		float uvCenterDistance = distance(uv, vec2(0.5, 0.5));
		if (uvCenterDistance < minimumUVCenterDistance)
		{
			nearUVCenterFrame = i;
			minimumUVCenterDistance = uvCenterDistance;
		}
	}

	//vec2 uv = WorldToUV(minimumDistanceFrame, worldPosition);
	//FragColor = texture(textureArray, vec3(uv, minimumDistanceFrame));

	vec2 uv = WorldToUV(nearUVCenterFrame, worldPosition);
	FragColor = texture(textureArray, vec3(uv, nearUVCenterFrame));

	//FragColor = texture(textureArray, vec3(uv, 2)) + vec4(GetFrameDirection(incremental), 1);

	//FragColor = vec4(wp.x, wp.y, GetFX(incremental % frameCount) , 1);

	//FragColor = vec4(wp.x, wp.y, 0, 1);

	/*
	float frameIndex = floor(TexCoord.x);
	float u = TexCoord.x - floor(TexCoord.x);
	float iv = floor(TexCoord.y);
	float v = TexCoord.y - floor(TexCoord.y);

	//float float_texel = float(texture2D(customDataSampler, TexCoord.xy));

	//vec4 asdf = frameMatrices1[int(frameIndex)] * frameMatrices[int(frameIndex)] * vec4(u, v, frameIndex, 1) + fl * vec4(0, 0, float_texel, 1);

	////vec4 asdf = vec4(focalLengths[0], 0, 0, 1);

	//FragColor = texture(textureArray, vec3(u, v, frameIndex)) + asdf;


	float float_texel = texelFetch(customDataSampler, ivec2(incremental, 0), 0).r;
	FragColor = texelFetch(customDataSampler, ivec2(incremental, 0), 0);

	//FragColor = texture(textureArray, vec3(u, v, frameIndex));
	*/
}
