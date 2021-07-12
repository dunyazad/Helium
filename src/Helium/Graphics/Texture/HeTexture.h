#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeImage;

	class HeTexture : public HeObject
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void Bind();
		void Unbind();

		inline GLuint GetTextureID() { return textureID; }

		inline GLsizei GetWidth() { return width; }
		inline GLsizei GetHeight() { return height; }

	protected:
		HeTexture(const string& name, HeImage* image);
		HeTexture(const string& name, int width, int height);
		~HeTexture();

		HeImage* image = nullptr;

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