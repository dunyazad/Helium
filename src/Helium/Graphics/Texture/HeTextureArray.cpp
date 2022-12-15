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

		if (textureData != nullptr) {
			delete textureData;
			textureData = nullptr;
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

		int totalLength = 0;
		for (auto& image : images)
		{
			totalLength += image->GetWidth() * image->GetHeight() * image->GetChannels();
		}
		
		textureData = new unsigned char[totalLength];
		memset(textureData, 255, totalLength);

		int index = 0;
		for (auto& image : images)
		{
			int length = image->GetWidth() * image->GetHeight() * image->GetChannels();
			memcpy(textureData + index, image->Data(), length);
			index += length;
		}

		glGenTextures(1, &textureID);

		glBindTexture(textureTarget, textureID);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, borderColor);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage3D(textureTarget, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, textureData);

		glGenerateMipmap(textureTarget);

		glTexParameteri(textureTarget, GL_TEXTURE_MAX_LEVEL, 4);

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
