#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

namespace ArtificialNature {

	class HeScene;
	class HeGeometry;

	class HeSceneNode : public HeObject
	{
	public:
		HeSceneNode(HeScene* scene);
		~HeSceneNode();

		inline HeSceneNode* GetParentNode() { return parentNode; }

		void AddChild(HeSceneNode* child);
		void RemoveChild(HeSceneNode* child);

		void Update(float dt);
		void Render();

		inline void AddGeometry(HeGeometry* geometry) { this->geometries.insert(geometry); }
		inline void RemoveGeometry(HeGeometry* geometry) { this->geometries.erase(geometry); }

	protected:
		HeScene* scene = nullptr;
		HeSceneNode* parentNode = nullptr;
		set<HeSceneNode*> childNodes;

		set<HeGeometry*> geometries;

		glm::quat localRotation;
		glm::vec3 localPosition;
		
		glm::quat absoluteRotation;
		glm::vec3 absolutePosition;
	private:

	};

}