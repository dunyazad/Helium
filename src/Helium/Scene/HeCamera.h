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

	class HeOrthogonalCamera;
	class HePerspectiveCamera;

	class HeCamera : public HeSceneNode
	{
	public:
		enum ProjectionMode { Perspective, Orthogonal };

	public:
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

		inline float GetAspectRatio() { return aspectRatio; }
		inline void SetAspectRatio(float aspectRatio) { this->aspectRatio = aspectRatio; }

		inline float GetZoomFactor() { return zoomFactor; }
		inline void SetZoomFactor(float zoomFactor) { this->zoomFactor = zoomFactor; }

		inline const glm::vec3& GetTargetPosition() const { return targetPosition; }
		inline void SetTargetPosition(const glm::vec3& targetPosition) { this->targetPosition = targetPosition; }

		inline const glm::mat4& GetViewMatrix() const { return viewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }

		inline glm::vec3 GetCameraRight() const { return glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]); }
		inline glm::vec3 GetCameraUp() const { return glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]); }
		inline glm::vec3 GetCameraFront() const { return -glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]); }

	protected:
		HeCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HeCamera(); 
		
		float viewportX = 0;
		float viewportY = 0;
		float viewportWidth = 800;
		float viewportHeight = 800;

		float aspectRatio = 1;
		float zoomFactor = 1;

		glm::vec3 targetPosition;
		glm::vec3 upDirection;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

	public:
		friend class HeScene;
	};

	class HeOrthogonalCamera : public HeCamera
	{
	public:
		virtual void Update(float dt);
		virtual void Render();

	protected:
		HeOrthogonalCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HeOrthogonalCamera(); 
		
		float width = 1;
		float height = 1;
		float depth = 1;

	public:
		friend class HeScene;
	};

	class HePerspectiveCamera : public HeCamera
	{
	public:
		virtual void Update(float dt);
		virtual void Render();
	protected:
		HePerspectiveCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight);
		~HePerspectiveCamera();

	public:
		friend class HeScene;
	};
}
