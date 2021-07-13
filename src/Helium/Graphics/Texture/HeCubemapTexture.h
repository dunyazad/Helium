#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Texture/HeTexture.h>

namespace ArtificialNature {

	class HeCubeMapTexture : public HeTexture
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

	protected:
		HeCubeMapTexture(const string& name, const vector<HeImage*>& images);
		~HeCubeMapTexture();

		vector<HeImage*> images;

	public:
		friend class HeGraphics;
	};

}