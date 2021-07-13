#include <Helium/Graphics/Texture/HeTexture.h>

#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeTexture::HeTexture(const string& name, HeImage* image)
		: HeObject(name), image(image)
	{
	}

	HeTexture::HeTexture(const string& name, int width, int height)
		: HeObject(name), width(width), height(height)
	{
	}

	HeTexture::~HeTexture()
	{
	}

	void HeTexture::Initialize()
	{
		if (image != nullptr)
		{
			width = image->GetWidth();
			height = image->GetHeight();
			withAlpha = image->GetChannels() == 4;

			if (width != 0 && height != 0)
			{
				textureData = new unsigned char[width * height * image->GetChannels()];
				memset(textureData, 255, width * height * image->GetChannels());
				memcpy(textureData, image->Data(), width * height * image->GetChannels());
			}
		}

		glGenTextures(1, &textureID);

		glBindTexture(target, textureID);

		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

		if (image == nullptr)
		{
			glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		else
		{
			if (textureData != nullptr)
			{
				if (withAlpha)
				{
					glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
				}
				else
				{
					glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				}
			}
		}
	}

	void HeTexture::Terminate()
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

	void HeTexture::Bind()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(target, textureID);
	}

	void HeTexture::Unbind()
	{
		glBindTexture(target, 0);
	}

	void HeTexture::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;

		if (width != 0 && height != 0)
		{
			if (textureData != nullptr)
			{
				delete textureData;
				textureData = nullptr;
			}

			if (image != nullptr)
			{
				textureData = new unsigned char[width * height * image->GetChannels()];
				memset(textureData, 255, width * height * image->GetChannels());
				memcpy(textureData, image->Data(), width * height * image->GetChannels());
			}
		}

		glBindTexture(target, textureID);

		if (image == nullptr)
		{
			glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		else
		{
			if (textureData != nullptr) {
				if (withAlpha) {
					glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
				}
				else {
					glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				}
			}
		}
	}
}
