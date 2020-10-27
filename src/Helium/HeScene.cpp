#include "HeScene.h"
#include "HeSceneNode.h"

namespace ArtificialNature {

	HeScene::HeScene()
	{
		rootNode = new HeSceneNode(this);
	}

	HeScene::~HeScene()
	{
	}

	void HeScene::Update(float dt)
	{
		rootNode->Update(dt);
	}

	void HeScene::Render()
	{
		rootNode->Render();
	}

}
