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

		void Bind(GLenum textureSlot, GLenum textureTarget)
		{
			glActiveTexture(textureSlot);
			glBindTexture(textureTarget, id);
		}

	protected:
		unsigned int id;
	};

}