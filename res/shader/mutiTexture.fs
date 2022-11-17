#version 330

out vec4 FragColor;

in vec4 ourColor;
in vec2 TexCoord;

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

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	float weight0 = 1.0;
	float weight1 = 0.0;
	float weight2 = 0.0;
	float weight3 = 0.0;
	float weight4 = 0.0;
	float weight5 = 0.0;
	float weight6 = 0.0;
	float weight7 = 0.0;
	float weight8 = 0.0;
	float weight9 = 0.0;
	float weight10 = 0.0;
	float weight11 = 0.0;
	float weight12 = 0.0;
	float weight13 = 0.0;
	float weight14 = 0.0;
	float weight15 = 0.0;

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

	FragColor = 
		textureColor0 * weight0 +
		textureColor1 * weight1 +
		textureColor2 * weight2 +
		textureColor3 * weight3 +
		textureColor4 * weight4 +
		textureColor5 * weight5 +
		textureColor6 * weight6 +
		textureColor7 * weight7 +
		textureColor8 * weight8 +
		textureColor9 * weight9 +
		textureColor10 * weight10 +
		textureColor11 * weight11 +
		textureColor12 * weight12 +
		textureColor13 * weight13 +
		textureColor14 * weight14 +
		textureColor15 * weight15;
}
