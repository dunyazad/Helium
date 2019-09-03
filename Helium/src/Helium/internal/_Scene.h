#pragma once

#include <Helium/Helium.h>
#include <Helium/Scene.h>

namespace ArtificialNature {

	class GraphicsDevice;

	class _Scene : public Scene
	{
	public:
		_Scene();
		~_Scene();

		virtual bool IsActive();
		virtual void SetActive(bool bActive);

		virtual void Render(GraphicsDevice* pGraphicsDevice);

	private:
		bool m_bActive = true;
	};

}
