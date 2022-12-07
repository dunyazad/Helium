#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeGeometrySkybox : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

	protected:
		HeGeometrySkybox(const string& name);
		~HeGeometrySkybox();

	public:
		friend class HeGraphics;
	};

}
