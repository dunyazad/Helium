#include <Helium/Scene/HeScene.h>

#include <Helium/Scene/HeSceneNode.h>
#include <Helium/Scene/HeSceneNodeImgui.h>
#include <Helium/Scene/HeCamera.h>
#include <Helium/Scene/HeCameraManipulator.h>
#include <Helium/Scene/HeVisualDebugger.h>

namespace ArtificialNature {

	HeScene::HeScene(const string& name, HeGraphics* graphics)
		: HeObject(name), graphics(graphics)
	{
		rootNode = new HeSceneNode("Root Node", this);
		imguiRootNode = new HeSceneNodeImgui("imgui", this);

		visualDebugger = new HeVisualDebugger(this, graphics);
	}

	HeScene::~HeScene()
	{
		for (auto& kvp : cameraManipulators)
		{
			HeDelete(kvp.second);
		}
		cameraManipulators.clear();

		if (rootNode != nullptr)
		{
			delete rootNode;
			rootNode = nullptr;
		}

		if (imguiRootNode != nullptr)
		{
			delete imguiRootNode;
			imguiRootNode = nullptr;
		}
	}

	void HeScene::Update(float dt)
	{
		rootNode->Update(dt);
	}

	void HeScene::UpdateImgui(float dt)
	{
		imguiRootNode->Update(dt);
	}

	void HeScene::Render()
	{
		rootNode->Render(mainCamera);
	}

	void HeScene::RenderImgui()
	{
		imguiRootNode->Render(mainCamera);
	}

	HeSceneNode* HeScene::CreateSceneNode(const string& name)
	{
		auto pNode = new HeSceneNode(name, this);
		rootNode->AddChild(pNode);
		return pNode;
	}

	HeSceneNodeImgui* HeScene::CreateSceneNodeImgui(const string& name)
	{
		auto pNode = new HeSceneNodeImgui(name, this);
		imguiRootNode->AddChild(pNode);
		return pNode;
	}

	HeOrthogonalCamera* HeScene::CreateOrthogonalCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
	{
		auto pNode = new HeOrthogonalCamera(name, this, viewportX, viewportY, viewportWidth, viewportHeight);
		rootNode->AddChild(pNode);
		return pNode;
	}

	HePerspectiveCamera* HeScene::CreatePerspectiveCamera(const string& name, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
	{
		auto pNode = new HePerspectiveCamera(name, this, viewportX, viewportY, viewportWidth, viewportHeight);
		rootNode->AddChild(pNode);
		return pNode;
	}

	HeCameraManipulatorOrtho* HeScene::CreateCameraManipulatorOrtho(const string& name, HeCamera* camera)
	{
		auto pCamera = dynamic_cast<HeOrthogonalCamera*>(camera);
		if (pCamera == nullptr)
		{
			return nullptr;
		}

		if (cameraManipulators.contains(name))
		{
			return dynamic_cast<HeCameraManipulatorOrtho*>(cameraManipulators[name]);
		}
		else
		{
			auto pManipulator = new HeCameraManipulatorOrtho(pCamera);
			cameraManipulators[name] = pManipulator;
			return pManipulator;
		}
	}

	HeCameraManipulatorFlight* HeScene::CreateCameraManipulatoFlight(const string& name, HeCamera* camera)
	{
		if (cameraManipulators.contains(name))
		{
			return dynamic_cast<HeCameraManipulatorFlight*>(cameraManipulators[name]);
		}
		else
		{
			auto pManipulator = new HeCameraManipulatorFlight(camera);
			cameraManipulators[name] = pManipulator;
			return pManipulator;
		}
	}

	HeCameraManipulatorTrackball* HeScene::CreateCameraManipulatorTrackball(const string& name, HeCamera* camera)
	{
		if (cameraManipulators.contains(name))
		{
			return dynamic_cast<HeCameraManipulatorTrackball*>(cameraManipulators[name]);
		}
		else
		{
			auto pManipulator = new HeCameraManipulatorTrackball(camera);
			cameraManipulators[name] = pManipulator;
			return pManipulator;
		}
	}

	HeSceneNode* HeScene::GetSceneNode(const string& name)
	{
		auto pNode = rootNode->GetSceneNode(name);
		if (pNode == nullptr) {
			pNode = imguiRootNode->GetSceneNode(name);
		}
		return pNode;
	}
}
