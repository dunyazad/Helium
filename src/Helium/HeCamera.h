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
		HeCamera(HeScene* scene);
		~HeCamera();

		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		inline const glm::mat4 GetViewMatrix() { return viewMatrix; }
		inline const glm::mat4 GetProjectionMatrix() { return projectionMatrix; }

	protected:
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};

	class HeOrthogonalCamera : public HeCamera
	{
	public:
		enum ProjectionMode { Perspective, Orthogonal };

	public:
		HeOrthogonalCamera(HeScene* scene);
		~HeOrthogonalCamera();

		virtual void Update(float dt);
		virtual void Render();

		inline void SetPosition(const glm::vec3& position) { this->position = position; }

		inline float GetZoomFactor() { return zoomFactor; }
		inline void SetZoomFactor(float zoomFactor) { this->zoomFactor = zoomFactor; if (this->zoomFactor <= glm::epsilon<float>()) this->zoomFactor = glm::epsilon<float>(); }

		inline float GetAspectRatio() { return aspectRatio; }
		inline void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

	protected:
		glm::vec3 position;
		float width = 1;
		float height = 1;
		float depth = 1;
		float aspectRatio = 1;
		float zoomFactor = 1;
	};

}