#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeImage;

	class HeTexture : public HeObject
	{
	public:
		void Initialize();
		bool Terminate();

		void Bind();
		void Unbind();

		inline GLuint GetTextureID() { return textureID; }

	protected:
		HeTexture(const string& name, HeImage* image);
		~HeTexture();

		HeImage* image;

		bool withAlpha = true;
		GLuint textureID;
		GLenum target = GL_TEXTURE_2D; // GL_TEXTURE_2D, GL_TEXTURE_EXTERNAL_OES
		GLenum format = GL_RGBA;
		GLsizei width = 0;
		GLsizei height = 0;
		GLenum dataType = GL_UNSIGNED_BYTE;
		unsigned char* textureData = nullptr;

	public:
		friend class HeGraphics;
	};

}