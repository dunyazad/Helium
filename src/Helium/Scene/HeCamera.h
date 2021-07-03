#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Scene/HeSceneNode.h>

namespace ArtificialNature {

	/*struct HeCameraState
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
	};*/

	class HeCamera : public HeSceneNode
	{
	public:
		enum ProjectionMode { Perspective, Orthogonal };

	public:
		HeCamera(HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HeCamera();

		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		virtual inline float GetZoomFactor() { return zoomFactor; }
		virtual inline void SetZoomFactor(float zoomFactor) { this->zoomFactor = zoomFactor; if (this->zoomFactor <= glm::epsilon<float>()) this->zoomFactor = glm::epsilon<float>(); }

		inline float GetAspectRatio() { return aspectRatio; }
		inline void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

		inline const glm::vec3& GetPosition() { return position; }
		inline void SetPosition(const glm::vec3& position) { this->position = position; }

		inline const glm::vec3& GetTargetPosition() { return targetPosition; }
		inline void SetTargetPosition(const glm::vec3& targetPosition) { this->targetPosition = targetPosition; }

		inline const glm::mat4 GetViewMatrix() { return viewMatrix; }
		inline const glm::mat4 GetProjectionMatrix() { return projectionMatrix; }

	protected:
		float viewportX = 0;
		float viewportY = 0;
		float viewportWidth = 800;
		float viewportHeight = 800;

		float aspectRatio = 1;
		float zoomFactor = 1;

		glm::vec3 position;
		glm::vec3 targetPosition;
		glm::vec3 upDirection;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
	};

	class HeOrthogonalCamera : public HeCamera
	{
	public:
		HeOrthogonalCamera(HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HeOrthogonalCamera();

		virtual void Update(float dt);
		virtual void Render();

	protected:
		float width = 1;
		float height = 1;
		float depth = 1;
	};

	class HePerspectiveCamera : public HeCamera
	{
	public:
		HePerspectiveCamera(HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HePerspectiveCamera();

		virtual void Update(float dt);
		virtual void Render();

		virtual inline float GetZoomFactor()
		{ 
			return glm::distance(targetPosition, position);
		}

		virtual inline void SetZoomFactor(float zoomFactor)
		{
			this->zoomFactor = zoomFactor;
			if (this->zoomFactor <= glm::epsilon<float>())
				this->zoomFactor = glm::epsilon<float>();

			auto dir = targetPosition - position;
			dir = glm::normalize(dir);
			SetPosition(targetPosition - dir * zoomFactor);
		}
	};
}
