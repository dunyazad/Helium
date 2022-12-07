#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeGraphics;
	class HeImage;
	class HeFrustum;
	class HeCameraInfo;
	class HeFrameInfo;
	class HeProject;
	
	class HeCameraInfo
	{
	public:
		HeCameraInfo(HeFrameInfo* frameInfo, const string& cameraInfoFile);
		HeCameraInfo(HeFrameInfo* frameInfo, const string& cameraInfoFile, const json& reconstructionInfo);

		~HeCameraInfo();

		glm::vec2 WorldToUV(const glm::vec3& worldPosition) const;

		inline int GetFrameIndex() const { return frameIndex; }
		inline int GetImageWidth() const { return imageWidth; }
		inline int GetImageHeight() const { return imageHeight; }
		inline float GetFX() const { return fx; }
		inline float GetFY() const { return fy; }
		inline float GetOX() const { return ox; }
		inline float GetOY() const { return oy; }

		inline const glm::mat3& GetIntrinsicMatrix() const { return intrinsicMatrix; }
		inline const glm::mat4& GetViewMatrix() const { return viewMatrix; }
		inline const glm::mat4& GetInverseViewMatrix() const { return viewMatrixInversed; }
		inline const glm::mat4& GetTransformMatrix() const { return transformMatrix; }
		inline const glm::mat4& GetInversedTransformMatrix() const { return transformMatrixInversed; }
		inline const glm::mat4& GetLocalToWorldMatrix() const { return localToWorldMatrix; }
		inline const glm::mat4& GetExtrinsicMatrix() const { return extrinsicMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; }

		inline const glm::vec3& GetPosition() const { return position; }
		inline const glm::mat3& GetRotation() const { return rotation; }

		inline const HeFrustum* GetFrustum() const { return frustum;}

	protected:
		int frameIndex = -1;
		int imageWidth = 0;
		int imageHeight = 0;
		float fx = 0.0f;
		float fy = 0.0f;
		float ox = 0.0f;
		float oy = 0.0f;
		glm::mat3 intrinsicMatrix = glm::identity<glm::mat3>();
		glm::mat4 viewMatrix = glm::identity<glm::mat4>();
		glm::mat4 viewMatrixInversed = glm::identity<glm::mat4>();
		glm::mat4 transformMatrix = glm::identity<glm::mat4>();
		glm::mat4 transformMatrixInversed = glm::identity<glm::mat4>();
		glm::mat4 localToWorldMatrix = glm::identity<glm::mat4>();
		glm::mat4 extrinsicMatrix = glm::identity<glm::mat4>();
		glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
		glm::mat4 viewProjectionMatrix = glm::identity<glm::mat4>();

		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::mat3 rotation = glm::identity<glm::mat3>();

		HeFrustum* frustum = nullptr;
	};
}
