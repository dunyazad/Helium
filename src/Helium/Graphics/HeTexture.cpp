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
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (image->Data() != nullptr)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->Width(), image->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->Data());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	void HeTexture::Terminate()
	{
		glDeleteTextures(1, &id);
	}
}
