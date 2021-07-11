#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Texture/HeTexture.h>

namespace ArtificialNature {

	class HeCanvasTexture : public HeTexture
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void Bind();
		void Unbind();

	protected:
		HeCanvasTexture(const string& name, HeImage* image);
		~HeCanvasTexture();

	public:
		friend class HeGraphics;
	};

}