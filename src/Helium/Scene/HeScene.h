#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {
	class HeSceneNode;
	class HeCamera;
	class HeOrthogonalCamera;
	class HePerspectiveCamera;

	class HeScene : public HeObject
	{
	public:
		inline HeSceneNode* GetRootNode() { return rootNode; }

		inline HeCamera* GetMainCamera() { return mainCamera; }
		inline void SetMainCamera(HeCamera* camera) { mainCamera = camera; }

		void Update(float dt);
		void Render();

		HeSceneNode* CreateSceneNode(const string& name);
		HeOrthogonalCamera* CreateOrthogonalCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		HePerspectiveCamera* CreatePerspectiveCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight);

	protected:
		HeScene(const string& name);
		~HeScene();

	private:
		HeSceneNode* rootNode = nullptr;
		HeCamera* mainCamera = nullptr;

	public:
		friend class Helium;
	};

}
