#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeGraphics;
	class HeTexture;

	class HeFrameBufferObject
	{
	public:
		virtual void Initialize();
		virtual void Terminate();
		void Bind();
		void Unbind();

		inline unsigned int ID() { return id; }
		inline HeTexture* GetTargetTexture() { return targetTexture; }

	protected:
		HeFrameBufferObject(HeGraphics* graphics, int width, int height);
		HeFrameBufferObject(HeGraphics* graphics, HeTexture* texture);
		~HeFrameBufferObject();

		HeGraphics* graphics;
		unsigned int id = -1;
		int width = 512;
		int height = 512;
		HeTexture* targetTexture = nullptr;

	public:
		friend class HeGraphics;
	};
}