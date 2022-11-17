#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

uniform float[32] textureWeights;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture7;
uniform sampler2D texture8;
uniform sampler2D texture9;
uniform sampler2D texture10;
uniform sampler2D texture11;
uniform sampler2D texture12;
uniform sampler2D texture13;
uniform sampler2D texture14;
uniform sampler2D texture15;
uniform sampler2D texture16;
uniform sampler2D texture17;
uniform sampler2D texture18;
uniform sampler2D texture19;
uniform sampler2D texture20;
uniform sampler2D texture21;
uniform sampler2D texture22;
uniform sampler2D texture23;
uniform sampler2D texture24;
uniform sampler2D texture25;
uniform sampler2D texture26;
uniform sampler2D texture27;
uniform sampler2D texture28;
uniform sampler2D texture29;
uniform sampler2D texture30;
uniform sampler2D texture31;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	vec4 textureColor0 = texture(texture0, TexCoord);
	vec4 textureColor1 = texture(texture1, TexCoord);
	vec4 textureColor2 = texture(texture2, TexCoord);
	vec4 textureColor3 = texture(texture3, TexCoord);
	vec4 textureColor4 = texture(texture4, TexCoord);
	vec4 textureColor5 = texture(texture5, TexCoord);
	vec4 textureColor6 = texture(texture6, TexCoord);
	vec4 textureColor7 = texture(texture7, TexCoord);
	vec4 textureColor8 = texture(texture8, TexCoord);
	vec4 textureColor9 = texture(texture9, TexCoord);
	vec4 textureColor10 = texture(texture10, TexCoord);
	vec4 textureColor11 = texture(texture11, TexCoord);
	vec4 textureColor12 = texture(texture12, TexCoord);
	vec4 textureColor13 = texture(texture13, TexCoord);
	vec4 textureColor14 = texture(texture14, TexCoord);
	vec4 textureColor15 = texture(texture15, TexCoord);
	vec4 textureColor16 = texture(texture16, TexCoord);
	vec4 textureColor17 = texture(texture17, TexCoord);
	vec4 textureColor18 = texture(texture18, TexCoord);
	vec4 textureColor19 = texture(texture19, TexCoord);
	vec4 textureColor20 = texture(texture20, TexCoord);
	vec4 textureColor21 = texture(texture21, TexCoord);
	vec4 textureColor22 = texture(texture22, TexCoord);
	vec4 textureColor23 = texture(texture23, TexCoord);
	vec4 textureColor24 = texture(texture24, TexCoord);
	vec4 textureColor25 = texture(texture25, TexCoord);
	vec4 textureColor26 = texture(texture26, TexCoord);
	vec4 textureColor27 = texture(texture27, TexCoord);
	vec4 textureColor28 = texture(texture28, TexCoord);
	vec4 textureColor29 = texture(texture29, TexCoord);
	vec4 textureColor30 = texture(texture30, TexCoord);
	vec4 textureColor31 = texture(texture31, TexCoord);

	FragColor = 
		textureColor0 * textureWeights[0] +
		textureColor1 * textureWeights[1] +
		textureColor2 * textureWeights[2] +
		textureColor3 * textureWeights[3] +
		textureColor4 * textureWeights[4] +
		textureColor5 * textureWeights[5] +
		textureColor6 * textureWeights[6] +
		textureColor7 * textureWeights[7] +
		textureColor8 * textureWeights[8] +
		textureColor9 * textureWeights[9] +
		textureColor10 * textureWeights[10] +
		textureColor11 * textureWeights[11] +
		textureColor12 * textureWeights[12] +
		textureColor13 * textureWeights[13] +
		textureColor14 * textureWeights[14] +
		textureColor15 * textureWeights[15] +
		textureColor16 * textureWeights[16] +
		textureColor17 * textureWeights[17] +
		textureColor18 * textureWeights[18] +
		textureColor19 * textureWeights[19] +
		textureColor20 * textureWeights[20] +
		textureColor21 * textureWeights[21] +
		textureColor22 * textureWeights[22] +
		textureColor23 * textureWeights[23] +
		textureColor24 * textureWeights[24] +
		textureColor25 * textureWeights[25] +
		textureColor26 * textureWeights[26] +
		textureColor27 * textureWeights[27] +
		textureColor28 * textureWeights[28] +
		textureColor29 * textureWeights[29] +
		textureColor30 * textureWeights[30] +
		textureColor31 * textureWeights[31];
}
