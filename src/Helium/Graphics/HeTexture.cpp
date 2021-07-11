#include <Helium/Graphics/HeTexture.h>

#include <Helium/Graphics/HeImage.h>

namespace ArtificialNature {

	HeTexture::HeTexture()
	{

	}

	HeTexture::~HeTexture()
	{

	}

	void HeTexture::Initialize(HeImage* image)
	{
		//glGenTextures(1, &id);
		//glBindTexture(GL_TEXTURE_2D, id);
		//
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//if (image->Data() != nullptr)
		//{
		//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->Width(), image->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->Data());
		//	glGenerateMipmap(GL_TEXTURE_2D);
		//}

		width = image->Width();
		height = image->Height();

		if (width != 0 && height != 0) {
			textureData = new unsigned char[width * height * 4];
			memset(textureData, 255, width * height * 4);
			memcpy(textureData, image->Data(), width * height * 4);
		}

		glGenTextures(1, &textureID);

		glBindTexture(target, textureID);

		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

		if (textureData != nullptr) {
			if (withAlpha) {
				glTexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			}
			else {
				glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			}
		}
	}

	bool HeTexture::Terminate()
	{
		glDeleteTextures(1, &textureID);

		if (textureData != nullptr) {
			delete textureData;
			textureData = nullptr;
		}

		return true;
	}

	void HeTexture::Bind() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(target, textureID);
	}

	void HeTexture::Unbind() {
		glBindTexture(target, 0);
	}
}