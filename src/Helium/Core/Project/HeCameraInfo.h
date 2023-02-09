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
		glm::vec3 UVToWorld(const glm::vec2& uv) const;

		inline int GetFrameIndex() const { return frameIndex; }
		inline int GetColorImageWidth() const { return colorImageWidth; }
		inline int GetColorImageHeight() const { return colorImageHeight; }
		inline int GetDepthImageWidth() const { return depthImageWidth; }
		inline int GetDepthImageHeight() const { return depthImageHeight; }
		inline float GetOriginalFX() const { return original_fx; }
		inline float GetOriginalFY() const { return original_fy; }
		inline float GetOriginalOX() const { return original_ox; }
		inline float GetOriginalOY() const { return original_oy; }
		inline float GetScaledFX() const { return scaled_fx; }
		inline float GetScaledFY() const { return scaled_fy; }
		inline float GetScaledOX() const { return scaled_ox; }
		inline float GetScaledOY() const { return scaled_oy; }

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

		inline float GetAmbientColorTemperature() const { return ambientColorTemperature; }
		inline float GetAmbientIntensity() const { return ambientIntensity; }

	protected:
		int frameIndex = -1;
		int colorImageWidth = 0;
		int colorImageHeight = 0;
		int depthImageWidth = 0;
		int depthImageHeight = 0;
		float original_fx = 0.0f;
		float original_fy = 0.0f;
		float original_ox = 0.0f;
		float original_oy = 0.0f;
		float scaled_fx = 0.0f;
		float scaled_fy = 0.0f;
		float scaled_ox = 0.0f;
		float scaled_oy = 0.0f;
		glm::mat3 intrinsicMatrix = glm::identity<glm::mat3>();
		glm::mat4 viewMatrix = glm::identity<glm::mat4>();
		glm::mat4 viewMatrixInversed = glm::identity<glm::mat4>();
		glm::mat4 transformMatrix = glm::identity<glm::mat4>();
		glm::mat4 transformMatrixInversed = glm::identity<glm::mat4>();
		glm::mat4 localToWorldMatrix = glm::identity<glm::mat4>();
		glm::mat4 extrinsicMatrix = glm::identity<glm::mat4>();
		glm::mat4 projectionMatrix = glm::identity<glm::mat4>();
		glm::mat4 viewProjectionMatrix = glm::identity<glm::mat4>();

		float ambientColorTemperature = 0.0f;
		float ambientIntensity = 0.0f;

		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::mat3 rotation = glm::identity<glm::mat3>();

		HeFrustum* frustum = nullptr;
	};
}
