#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

namespace ArtificialNature {

	class HeScene;

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

	protected:
		HeScene* scene = nullptr;
		HeSceneNode* parentNode = nullptr;
		set<HeSceneNode*> childNodes;

	private:

	};

}