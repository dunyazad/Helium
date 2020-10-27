#include "HeSceneNode.h"
#include "HeScene.h"

namespace ArtificialNature {

	HeSceneNode::HeSceneNode(HeScene* rootNode)
		: scene(scene)
	{
	}

	HeSceneNode::~HeSceneNode()
	{
		for (auto& child : childNodes)
		{
			delete child;
		}
		childNodes.clear();
	}

	void HeSceneNode::AddChild(HeSceneNode* child)
	{
		if (child->parentNode != nullptr)
		{
			child->parentNode->childNodes.erase(child);
		}
		child->parentNode = this;
		childNodes.insert(child);
	}

	void HeSceneNode::RemoveChild(HeSceneNode* child)
	{
		scene->GetRootNode()->AddChild(child);
	}

	void HeSceneNode::Update(float dt)
	{
		for (auto& child : childNodes)
		{
			child->Update(dt);
		}
	}

	void HeSceneNode::Render()
	{
		for (auto& child : childNodes)
		{
			child->Render();
		}
	}

}
