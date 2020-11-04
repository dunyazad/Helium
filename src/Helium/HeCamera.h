#pragma once

#include "HeliumCommon.h"
#include "HeSceneNode.h"

namespace ArtificialNature {

	struct HeCameraState
	{
		enum ProjectionMode { ePerspective, eOrthogonal };
		ProjectionMode projectionMode;

		float fovy;
		float zNear;
		float zFar;

		HeSceneNode* pTargetSceneNode;
		glm::vec3 targetPosition;

		float rotationH;
		float rotationV;
		float distance;
	};

	class HeCamera : public HeSceneNode
	{
	public:
		enum ProjectionMode { Perspective, Orthogonal };

	public:
		HeCamera(HeScene* scene);
		~HeCamera();

		void Update(float dt);
		void Render();

	protected:
		int viewportX;
		int viewportY;
		int viewportWidth;
		int viewportHeight;


		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 upVector;
	};

}