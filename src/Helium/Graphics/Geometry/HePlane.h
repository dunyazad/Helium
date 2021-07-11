#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HePlane : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

	protected:
		HePlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type);
		~HePlane();

		float columnLength = 1.0f;
		float rowLength = 1.0f;
		unsigned int columns = 1;
		unsigned int rows = 1;
		HePlaneType type = XZ;

	public:
		friend class HeGraphics;
	};

}
