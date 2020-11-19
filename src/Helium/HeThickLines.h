#pragma once

#include "HeliumCommon.h"
#include "HeGeometry.h"

namespace ArtificialNature {

	class HeThickLines : public HeGeometry
	{
	public:
		HeThickLines();
		~HeThickLines();

		void Initialize();
		void Terminate();

		inline void SetThickness(float thickness) { this->thickness = thickness; }

	protected:
		float thickness = 0.1f;
	};

}
