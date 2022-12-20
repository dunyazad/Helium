#include <Helium/Graphics/Texture/HeTextureFloatData.h>

#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeTextureFloatData::HeTextureFloatData(const string& name, float* data, int dataCount)
		: HeTexture(name, nullptr)
	{
		this->data = new float[dataCount];
		memcpy(this->data, data, sizeof(float) * dataCount);
		this->dataCount = dataCount;
	}

	HeTextureFloatData::HeTextureFloatData(const string& name, const vector<float>& data)
		: HeTexture(name, nullptr)
	{
		this->data = new float[data.size()];
		memcpy(this->data, &(data[0]), sizeof(float) * data.size());
		this->dataCount = data.size();
	}

	HeTextureFloatData::~HeTextureFloatData()
	{
		if (data != nullptr)
		{
			delete data;
			data = nullptr;
		}
	}

	void HeTextureFloatData::Initialize()
	{
		glGenTextures(1, &textureID);

		glBindTexture(textureTarget, textureID);

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(textureTarget, 0, GL_R32F, dataCount, 1, 0, GL_RED, GL_FLOAT, data);

		CheckGLError();
	}

}
