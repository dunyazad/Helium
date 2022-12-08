#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Texture/HeTexture.h>

namespace ArtificialNature {

	class HeCubemapTexture : public HeTexture
	{
	public:
		virtual void Initialize();

	protected:
		HeCubemapTexture(const string& name, const vector<HeImage*>& images);
		~HeCubemapTexture();

		vector<HeImage*> images;

	public:
		friend class HeGraphics;
	};

}