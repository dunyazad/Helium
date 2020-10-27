#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

namespace ArtificialNature {
	class HeSceneNode;

	class HeScene : public HeObject
	{
	public:
		HeScene();
		~HeScene();

		inline HeSceneNode* GetRootNode() { return rootNode; }

		void Update(float dt);
		void Render();

	private:
		HeSceneNode* rootNode;
	};

}
