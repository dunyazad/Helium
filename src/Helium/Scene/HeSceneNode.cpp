#include <Helium/Scene/HeSceneNode.h>

#include <Helium/Scene/HeScene.h>
#include <Helium/Scene/HeCamera.h>

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
		for (auto& callback : onPreupdateEventHandlers)
		{
			callback->t(this, dt);
		}

		if (parentNode != nullptr) {
			absolutePosition = parentNode->absolutePosition + parentNode->absoluteRotation * localPosition;
			absoluteRotation = parentNode->absoluteRotation * localRotation;
			absoluteScale = parentNode->absoluteScale * localScale;
		}
		else {
			absolutePosition = localPosition;
			absoluteRotation = localRotation;
			absoluteScale = localScale;
		}

		glm::mat4 model = glm::toMat4(absoluteRotation);
		model[3][0] = absolutePosition.x;
		model[3][1] = absolutePosition.y;
		model[3][2] = absolutePosition.z;
		glm::mat4 scaleM = glm::identity<glm::mat4>();
		scaleM[0][0] = absoluteScale.x;
		scaleM[1][1] = absoluteScale.y;
		scaleM[2][2] = absoluteScale.z;
		absoluteTransform = model * scaleM;

		for (auto& child : childNodes)
		{
			child->Update(dt);
		}

		for (auto& callback : onPostupdateEventHandlers)
		{
			callback->t(this, dt);
		}
	}

	void HeSceneNode::Render(HeCamera* camera)
	{
		for (auto& geometry : geometries)
		{
			if (geometry != nullptr)
			{
				geometry->Draw(camera->GetProjectionMatrix(), camera->GetViewMatrix(), absoluteTransform);
			}
		}

		for (auto& child : childNodes)
		{
			child->Render(camera);
		}
	}

	HeCallback<HeSceneNodeUpdateCallback>* HeSceneNode::AddOnPreupdate(HeSceneNodeUpdateCallback handler)
	{
		auto pCallback = new HeCallback<HeSceneNodeUpdateCallback>(handler);
		onPreupdateEventHandlers.insert(pCallback);
		return pCallback;
	}

	HeCallback<HeSceneNodeUpdateCallback>* HeSceneNode::AddOnPostupdate(HeSceneNodeUpdateCallback handler)
	{
		auto pCallback = new HeCallback<HeSceneNodeUpdateCallback>(handler);
		onPostupdateEventHandlers.insert(pCallback);
		return pCallback;
	}

	bool HeSceneNode::RemoveOnPreupdate(HeCallback<HeSceneNodeUpdateCallback>* handler)
	{
		if (onPreupdateEventHandlers.count(handler) != 0) {
			onPreupdateEventHandlers.erase(handler);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool HeSceneNode::RemoveOnPostupdate(HeCallback<HeSceneNodeUpdateCallback>* handler)
	{
		if (onPostupdateEventHandlers.count(handler) != 0) {
			onPostupdateEventHandlers.erase(handler);
			return true;
		}
		else
		{
			return false;
		}
	}
}
