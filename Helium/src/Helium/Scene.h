#pragma once

#include <Helium/HeliumCommon.h>

namespace ArtificialNature {

	class GraphicsDevice;

	class HELIUM_API Scene
	{
	public:
		virtual bool IsActive() = 0;
		virtual void SetActive(bool bActive) = 0;

		virtual void Render(GraphicsDevice* pGraphicsDevice) = 0;
	};

}
