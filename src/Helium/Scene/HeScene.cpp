#include <Helium/Scene/HeScene.h>

#include <Helium/Scene/HeSceneNode.h>
#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeScene::HeScene(const string& name)
		: HeObject(name)
	{
		rootNode = new HeSceneNode("Root Node", this);
	}

	HeScene::~HeScene()
	{
		if (rootNode != nullptr)
		{
			delete rootNode;
			rootNode = nullptr;
		}
	}

	void HeScene::Update(float dt)
	{
		rootNode->Update(dt);
	}

	void HeScene::Render()
	{
		rootNode->Render(mainCamera);
	}

	HeSceneNode* HeScene::CreateSceneNode(const string& name)
	{
		auto pNode = new HeSceneNode(name, this);
		rootNode->AddChild(pNode);
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

}
