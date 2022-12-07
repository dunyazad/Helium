#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {
	class HeGraphics;
	class HeSceneNode;
	class HeSceneNodeImgui;
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
		void UpdateImgui(float dt);
		void Render();
		void RenderImgui();

		HeSceneNode* CreateSceneNode(const string& name);
		HeSceneNodeImgui* CreateSceneNodeImgui(const string& name);
		HeOrthogonalCamera* CreateOrthogonalCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		HePerspectiveCamera* CreatePerspectiveCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight);

		HeSceneNode* GetSceneNode(const string& name);

		inline HeGraphics* GetGraphics() { return graphics; }

	protected:
		HeScene(const string& name, HeGraphics* graphics);
		~HeScene();

	private:
		HeGraphics* graphics = nullptr;
		HeSceneNode* rootNode = nullptr;
		HeSceneNode* imguiRootNode = nullptr;
		HeCamera* mainCamera = nullptr;

	public:
		friend class Helium;
	};

}
