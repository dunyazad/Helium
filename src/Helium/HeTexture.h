#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeImage;

	class HeTexture
	{
	public:
		HeTexture();
		~HeTexture();

		void Initialize(HeImage* image);
		void Terminate();

	protected:
		unsigned int id;
	};

}