#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/HeGeometry.h>

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
