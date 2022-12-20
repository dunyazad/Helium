#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Texture/HeTexture.h>

namespace ArtificialNature {

	class HeImage;

	class HeTextureFloatData : public HeTexture
	{
	public:
		virtual void Initialize();

	protected:
		HeTextureFloatData(const string& name, float* data, int dataCount);
		HeTextureFloatData(const string& name, const vector<float>& data);
		~HeTextureFloatData();

		float* data = nullptr;
		int dataCount = 0;

	public:
		friend class HeGraphics;
	};

}