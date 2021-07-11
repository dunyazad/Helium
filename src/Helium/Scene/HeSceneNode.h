#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeScene;
	class HeGeometry;
	class HeCamera;
	class HeSceneNode;

	typedef function<void(HeSceneNode*, float)> HeSceneNodeUpdateCallback;

	class HeSceneNode : public HeObject
	{
	public:
		HeSceneNode(HeScene* scene);
		~HeSceneNode();

		inline HeSceneNode* GetParentNode() { return parentNode; }

		void AddChild(HeSceneNode* child);
		void RemoveChild(HeSceneNode* child);

		virtual void Update(float dt);
		virtual void Render(HeCamera* camera);

		HeCallback<HeSceneNodeUpdateCallback>* AddOnPreupdate(HeSceneNodeUpdateCallback callback);
		HeCallback<HeSceneNodeUpdateCallback>* AddOnPostupdate(HeSceneNodeUpdateCallback callback);
		bool RemoveOnPreupdate(HeCallback<HeSceneNodeUpdateCallback>* callback);
		bool RemoveOnPostupdate(HeCallback<HeSceneNodeUpdateCallback>* callback);

		inline void AddGeometry(HeGeometry* geometry) { this->geometries.insert(geometry); }
		inline void RemoveGeometry(HeGeometry* geometry) { this->geometries.erase(geometry); }

		inline const glm::quat& GetLocalRotation() { return localRotation; }
		inline const glm::vec3& GetLocalPosition() { return localPosition; }
		inline const glm::vec3& GetLocalScale() { return localScale; }

		inline void SetLocalRotation(const glm::quat& rotation) { localRotation = rotation; }
		inline void SetLocalPosition(const glm::vec3& position) { localPosition = position; }
		inline void SetLocalScale(const glm::vec3& scale) { localScale = scale; }

		inline const glm::mat4& GetAbsoluteTransform() { return absoluteTransform; }

	protected:
		HeScene* scene = nullptr;
		HeSceneNode* parentNode = nullptr;
		set<HeSceneNode*> childNodes;

		set<HeGeometry*> geometries;

		glm::quat localRotation = glm::identity<glm::quat>();
		glm::vec3 localPosition = glm::vec3(0, 0, 0);
		glm::vec3 localScale = glm::vec3(1, 1, 1);
		
		glm::quat absoluteRotation = glm::identity<glm::quat>();
		glm::vec3 absolutePosition = glm::vec3(0, 0, 0);
		glm::vec3 absoluteScale = glm::vec3(1, 1, 1);

		glm::mat4 absoluteTransform = glm::identity<glm::mat4>();

	private:
		set<HeCallback<HeSceneNodeUpdateCallback>*> onPreupdateCallbacks;
		set<HeCallback<HeSceneNodeUpdateCallback>*> onPostupdateCallbacks;
	};

}