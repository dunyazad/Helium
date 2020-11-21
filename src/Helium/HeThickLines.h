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

		void SetThickness(float thickness);

	protected:
		float thickness = 1.0f;
	};

}
