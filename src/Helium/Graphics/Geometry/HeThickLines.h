#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeThickLines : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void SetThickness(float thickness);

	protected:
		HeThickLines(const string& name);
		~HeThickLines();

		float thickness = 1.0f;

	public:
		friend class HeGraphics;
	};

}
