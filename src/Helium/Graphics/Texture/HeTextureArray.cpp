#include <Helium/Graphics/Texture/HeTextureArray.h>

#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeTextureArray::HeTextureArray(const string& name, const vector<HeImage*>& images)
		: HeObject(name), images(images)
	{
	}

	HeTextureArray::~HeTextureArray()
	{
		if (textureID != -1)
		{
			glDeleteTextures(1, &textureID);
		}

		for (auto& textureData : textureDatas)
		{
			if (textureData != nullptr) {
				delete textureData;
				textureData = nullptr;
			}
		}
	}

	void HeTextureArray::Initialize()
	{
		if (images.size() == 0)
		{
			return;
		}

		width = images[0]->GetWidth();
		height = images[0]->GetHeight();
		depth = (GLsizei)images.size();
		channels = images[0]->GetChannels();
		if (channels == 3) format = GL_RGB;

		glGenTextures(1, &textureID);

		glBindTexture(textureTarget, textureID);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, borderColor);

		//glTextureStorage3D(textureID, 0, format, width, height, images.size());
		glTexStorage3D(textureTarget, 1, GL_RGBA8, width, height, images.size());
		CheckGLError();

		glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 4);

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		textureDatas.resize(images.size());
		for (size_t i = 0; i < images.size(); i++)
		{
			auto image = images[i];
			int length = image->GetWidth() * image->GetHeight() * image->GetChannels();
			textureDatas[i] = new unsigned char[length];
			auto textureData = textureDatas[i];
			memcpy(textureData, image->Data(), length);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			//glTexImage3D(textureTarget, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, textureData);
			glTexSubImage3D(textureTarget, 0, 0, 0, i, width, height, 1, format, GL_UNSIGNED_BYTE, textureData);

			CheckGLError();
		}

		glGenerateMipmap(textureTarget);
	}

	void HeTextureArray::Bind(GLenum textureSlot)
	{
		glActiveTexture(textureSlot);
		CheckGLError();

		glBindTexture(textureTarget, textureID);
		CheckGLError();
	}

	void HeTextureArray::Unbind()
	{
		glBindTexture(textureTarget, 0);
	}

}
