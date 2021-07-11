#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {
	class HeSceneNode;
	class HeCamera;

	class HeScene : public HeObject
	{
	public:
		HeScene();
		~HeScene();

		inline HeSceneNode* GetRootNode() { return rootNode; }

		inline HeCamera* GetMainCamera() { return mainCamera; }
		inline void SetMainCamera(HeCamera* camera) { mainCamera = camera; }

		void Update(float dt);
		void Render();

	private:
		HeSceneNode* rootNode = nullptr;
		HeCamera* mainCamera = nullptr;
	};

}
