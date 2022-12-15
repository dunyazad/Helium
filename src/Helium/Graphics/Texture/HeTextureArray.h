#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeImage;

	class HeTextureArray : public HeObject
	{
	public:
		virtual void Initialize();

		void Bind(GLenum textureSlot = GL_TEXTURE0);
		void Unbind();

		inline const vector<HeImage*>& GetImage() const { return images; }

		inline GLenum GetTarget() { return textureTarget; }
		inline GLuint GetTextureID() { return textureID; }

		inline GLsizei GetWidth() { return width; }
		inline GLsizei GetHeight() { return height; }
		inline GLsizei GetChannels() { return channels; }

		inline bool HasAlpha() { return withAlpha; }

	protected:
		HeTextureArray(const string& name, const vector<HeImage*>& images);
		~HeTextureArray();

		vector<HeImage*> images;

		bool withAlpha = true;
		GLuint textureID = -1;
		GLenum textureTarget = GL_TEXTURE_2D_ARRAY;
		GLenum format = GL_RGBA;
		GLsizei width = 0;
		GLsizei height = 0;
		GLsizei channels = 0;
		GLsizei depth = 0;
		GLenum dataType = GL_UNSIGNED_BYTE;
		unsigned char* textureData = nullptr;

	public:
		friend class HeGraphics;
	};

}