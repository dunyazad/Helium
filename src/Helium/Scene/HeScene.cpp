#include <Helium/Scene/HeScene.h>

#include <Helium/Scene/HeSceneNode.h>
#include <Helium/Scene/HeSceneNodeImgui.h>
#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeScene::HeScene(const string& name, HeGraphics* graphics)
		: HeObject(name), graphics(graphics)
	{
		rootNode = new HeSceneNode("Root Node", this);
		imguiRootNode = new HeSceneNodeImgui("imgui", this);
	}

	HeScene::~HeScene()
	{
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

	HeSceneNode* HeScene::GetSceneNode(const string& name)
	{
		auto pNode = rootNode->GetSceneNode(name);
		if (pNode == nullptr) {
			pNode = imguiRootNode->GetSceneNode(name);
		}
		return pNode;
	}
}
