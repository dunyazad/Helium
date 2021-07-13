#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeSkyboxGeometry : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

	protected:
		HeSkyboxGeometry(const string& name);
		~HeSkyboxGeometry();

	public:
		friend class HeGraphics;
	};

}
