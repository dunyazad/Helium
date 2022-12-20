#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeImage;

	class HeTexture : public HeObject
	{
	public:
		virtual void Initialize();

		void Bind(GLenum textureSlot = GL_TEXTURE0);
		void Unbind();

		virtual void Resize(int width, int height);

		inline HeImage* GetImage() const { return image; }

		inline GLenum GetTarget() { return textureTarget; }
		inline GLuint GetTextureID() { return textureID; }

		inline GLsizei GetWidth() { return width; }
		inline GLsizei GetHeight() { return height; }

		inline bool HasAlpha() { return withAlpha; }

	protected:
		HeTexture(const string& name, HeImage* image);
		HeTexture(const string& name, int width, int height);
		~HeTexture();

		HeImage* image = nullptr;

		bool withAlpha = true;
		GLuint textureID = -1;
		GLenum textureTarget = GL_TEXTURE_2D; // GL_TEXTURE_2D, GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_CUBE_MAP
		GLenum format = GL_RGBA;
		GLsizei width = 0;
		GLsizei height = 0;
		GLenum dataType = GL_UNSIGNED_BYTE;
		unsigned char* textureData = nullptr;

	public:
		friend class HeGraphics;
	};

}