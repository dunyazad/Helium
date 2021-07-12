#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeGraphics;
	class HeTexture;

	class HeFrameBufferObject : public HeObject
	{
	public:
		virtual void Initialize();
		virtual void Terminate();
		void Bind();
		void Unbind();

		inline unsigned int GetFBOID() { return fboID; }
		inline HeTexture* GetTargetTexture() { return targetTexture; }

	protected:
		HeFrameBufferObject(const string& name, HeGraphics* graphics, int width, int height);
		HeFrameBufferObject(const string& name, HeGraphics* graphics, HeTexture* texture);
		~HeFrameBufferObject();

		HeGraphics* graphics;
		GLuint fboID = -1;
		GLuint depthBufferID = -1;
		int width = 512;
		int height = 512;
		HeTexture* targetTexture = nullptr;

	public:
		friend class HeGraphics;
	};
}