#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeGraphics;
	class HeImage;
	class HeFrustum;
	class HeCameraInfo;
	class HeFrameInfo;

	class HeProject
	{
	public:
		HeProject(const string& userName, const string& projectName);

		HeProject(const string& userName, const string& projectName, const filesystem::path& projectsRootPath);

		~HeProject();

		void FillOther();

		void CreateDirectoryIfNotExists(const filesystem::path& path);

		inline const filesystem::path& GetRootPath() const { return this->rootPath; }
		inline const filesystem::path& GetCameraInfoPath() const { return this->cameraInfoPath; }
		inline const filesystem::path& GetColorPath() const { return this->colorPath; }
		inline const filesystem::path& GetDepthPath() const { return this->depthPath; }
		inline const filesystem::path& GetResizedColorPath() const { return this->resizedColorPath; }
		inline const filesystem::path& GetResizedDepthPath() const { return this->resizedDepthPath; }
		inline const filesystem::path& GetConvertedDepthPath() const { return this->convertedDepthPath; }
		inline const filesystem::path& GetRoomPlanPath() const { return this->roomPlanPath; }
		inline const filesystem::path& GetReconstructedPath() const { return this->reconstructedPath; }
		inline const filesystem::path& GetSubVolumePath() const { return this->subVolumePath; }
		inline const filesystem::path& GetUnwrapPath() const { return this->unwrapPath; }
		inline const vector<HeFrameInfo*>& GetFrames() const { return this->frames; }

	protected:
		string userName;
		string projectName;
		filesystem::path rootPath;
		filesystem::path cameraInfoPath;
		filesystem::path colorPath;
		filesystem::path depthPath;
		filesystem::path resizedColorPath;
		filesystem::path resizedDepthPath;
		filesystem::path convertedDepthPath;
		filesystem::path roomPlanPath;
		filesystem::path reconstructedPath;
		filesystem::path subVolumePath;
		filesystem::path unwrapPath;

		vector<HeFrameInfo*> frames;
	};

	class HeFrameInfo
	{
	public:
		HeFrameInfo(HeProject* project, int frameIndex);
		HeFrameInfo(HeProject* project, int frameIndex, const json& reconstructionInfo);

		~HeFrameInfo();

		void LoadColorImage(HeGraphics* pGraphics);
		void LoadDepthInfo();

		inline int GetFrameIndex() const { return frameIndex; }
		inline const filesystem::path& GetCameraInfoFile() const { return cameraInfoFile; }
		inline const filesystem::path& GetDepthFile() const { return depthFile; }
		inline const filesystem::path& GetResizedDepthFile() const { return resizedDepthFile; }
		inline const filesystem::path& GetDepthDataFilePath() const { return depthDataFilePath; }
		inline const filesystem::path& GetConvertedDepthFile() const { return convertedDepthFile; }
		inline const filesystem::path& GetColorFile() const { return colorFile; }
		inline const filesystem::path& GetResizedColorFile() const { return resizedColorFile; }
		inline const HeCameraInfo* GetCameraInfo() const { return cameraInfo; }
		inline HeImage* GetColorImage() const { return colorImage; }
		inline const vector<float>& GetDepthInfos() const { return depthInfos; }

	protected:
		HeProject* project;
		int frameIndex;
		filesystem::path cameraInfoFile;
		filesystem::path depthFile;
		filesystem::path resizedDepthFile;
		filesystem::path depthDataFilePath;
		filesystem::path convertedDepthFile;
		filesystem::path colorFile;
		filesystem::path resizedColorFile;
		HeCameraInfo* cameraInfo;
		HeImage* colorImage;
		vector<float> depthInfos;
	};

	class HeCameraInfo
	{
	public:
		HeCameraInfo(HeFrameInfo* frameInfo, const filesystem::path& cameraInfoFile);
		HeCameraInfo(HeFrameInfo* frameInfo, const filesystem::path& cameraInfoFile, const json& reconstructionInfo);

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
