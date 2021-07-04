#include <Helium/Scene/HeSceneNode.h>

#include <Helium/Scene/HeScene.h>

#include <Helium/Graphics/HeGeometry.h>

namespace ArtificialNature {

	HeSceneNode::HeSceneNode(HeScene* rootNode)
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
		if (parentNode != nullptr)
		{
			absoluteRotation = parentNode->absoluteRotation * localRotation;
			absolutePosition = parentNode->absolutePosition + localRotation * localPosition;
		}
		else
		{
			absoluteRotation = localRotation;
			absolutePosition = localRotation * localPosition;
		}

		for (auto& child : childNodes)
		{
			child->Update(dt);
		}
	}

	void HeSceneNode::Render(HeCamera* camera)
	{
		for (auto& geometry : geometries)
		{
			if (geometry != nullptr)
			{
				geometry->Draw(camera);
			}
		}

		for (auto& child : childNodes)
		{
			child->Render(camera);
		}
	}

}
