#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HePlane : public HeGeometry
	{
	public:
		HePlane();
		~HePlane();

		void Initialize();
		void Terminate();

	protected:
	};

}
