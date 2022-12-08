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
		if (textureID != -1)
		{
			glDeleteTextures(1, &textureID);
		}

		if (textureData != nullptr) {
			delete textureData;
			textureData = nullptr;
		}
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
				if (image->Data() != nullptr)
				{
					memcpy(textureData, image->Data(), width * height * image->GetChannels());
				}
			}
		}

		glGenTextures(1, &textureID);

		glBindTexture(textureTarget, textureID);

		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(textureTarget, GL_TEXTURE_BORDER_COLOR, borderColor);

		if (image == nullptr)
		{
			glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		else
		{
			if (textureData != nullptr)
			{
				if (withAlpha)
				{
					glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
				}
				else
				{
					glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				}
			}
		}
	}

	void HeTexture::Bind(GLenum textureSlot)
	{
		glActiveTexture(textureSlot);
		CheckGLError();

		glBindTexture(textureTarget, textureID);
		CheckGLError();
	}

	void HeTexture::Unbind()
	{
		glBindTexture(textureTarget, 0);
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

		glBindTexture(textureTarget, textureID);

		if (image == nullptr)
		{
			glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		else
		{
			if (textureData != nullptr) {
				if (withAlpha) {
					glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
				}
				else {
					glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
				}
			}
		}
	}
}
