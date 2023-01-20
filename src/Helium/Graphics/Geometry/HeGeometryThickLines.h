#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeGeometryThickLines : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void SetThickness(float thickness);

	protected:
		HeGeometryThickLines(const string& name);
		~HeGeometryThickLines();

		float thickness = 1.0f;

	public:
		friend class HeGraphics;
	};

}
