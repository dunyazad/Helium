#include <Helium/Graphics/Texture/HeCubeMapTexture.h>

#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeCubeMapTexture::HeCubeMapTexture(const string& name, const vector<HeImage*>& images)
		: HeTexture(name, nullptr), images(images)
	{
		target = GL_TEXTURE_CUBE_MAP;
	}

	HeCubeMapTexture::~HeCubeMapTexture()
	{
	}

	void HeCubeMapTexture::Initialize()
	{
		glGenTextures(1, &textureID);

		glBindTexture(target, textureID);

		glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLfloat borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

		for (size_t i = 0; i < images.size(); i++)
		{
			auto& image = images[i];
			if (image->GetChannels() == 4)
			{
				glTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->Data());
			}
			else
			{
				glTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->Data());
			}
		}
	}

	void HeCubeMapTexture::Terminate()
	{
		HeTexture::Terminate();
	}
}
