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

		virtual void Update(float dt);
		virtual void Render();

		inline void SetProjectionMode(ProjectionMode projectionMode) { this->projectionMode = projectionMode; }

		inline const glm::mat4 GetViewMatrix() { return viewMatrix; }
		inline const glm::mat4 GetProjectionMatrix() { return projectionMatrix; }

	protected:
		int viewportX;
		int viewportY;
		int viewportWidth;
		int viewportHeight;

		ProjectionMode projectionMode = Perspective;


		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 upVector;
	};

}