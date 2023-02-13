#include <Helium/Core/Project/Project.h>
#include <Helium/Graphics/Graphics.h>

namespace ArtificialNature {

	HeFrameInfo::HeFrameInfo(HeProject* project, int frameIndex)
	{
		this->project = project;
		this->frameIndex = frameIndex;
		this->cameraInfoFile = project->GetCameraInfoPath() + format("/camera_info_{}.cam", frameIndex);
		this->depthFile = project->GetDepthPath() + format("/depth_{}.png", frameIndex);
		this->resizedDepthFile = project->GetResizedDepthPath() + format("/resized_depth_{}.png", frameIndex);
		this->depthDataFilePath = project->GetDepthPath() + format("/depth_{}.dpt", frameIndex);
		this->convertedDepthFile = project->GetConvertedDepthPath() + format("/depth_{}.png", frameIndex);
		this->colorFile = project->GetColorPath() + format("/color_{}.jpg", frameIndex);
		this->resizedColorFile = project->GetResizedColorPath() + format("/resized_color_{}.jpg", frameIndex);
		this->cameraInfo = new HeCameraInfo(this, this->cameraInfoFile);
	}

	HeFrameInfo::HeFrameInfo(HeProject* project, int frameIndex, const json& reconstructionInfo)
	{
		this->project = project;
		this->frameIndex = frameIndex;
		this->cameraInfoFile = project->GetCameraInfoPath() + format("/camera_info_{}.cam", frameIndex);
		this->depthFile = project->GetDepthPath() + format("/depth_{}.png", frameIndex);
		this->resizedDepthFile = project->GetResizedDepthPath() + format("/resized_depth_{}.png", frameIndex);
		this->depthDataFilePath = project->GetDepthPath() + format("/depth_{}.dpt", frameIndex);
		this->convertedDepthFile = project->GetConvertedDepthPath() + format("/depth_{}.png", frameIndex);
		this->colorFile = project->GetColorPath() + format("/color_{}.jpg", frameIndex);
		this->resizedColorFile = project->GetResizedColorPath() + format("/resized_color_{}.jpg", frameIndex);
		this->cameraInfo = new HeCameraInfo(this, this->cameraInfoFile, reconstructionInfo);

		LoadDepthInfo();
	}

	HeFrameInfo::~HeFrameInfo()
	{
		delete this->cameraInfo;
	}

	HeImage* HeFrameInfo::LoadColorImage(HeGraphics* pGraphics, bool verticalFlip)
	{
		stringstream ss;
		ss << "frame" << frameIndex;
		colorImage = pGraphics->GetImage(ss.str(), colorFile);
		colorImage->Initialize();
		return colorImage;
	}

	void HeFrameInfo::LoadDepthInfo()
	{
		auto depthPath = project->GetDepthPath();

		HeFile file;
		stringstream ss;
		ss << "depth_" << frameIndex << ".dpt";
		if (file.Open((depthPath + "/" + ss.str()), true)) {
			auto len = file.GetFileLength();
			depthInfos.resize(len / 4);
			file.Read((char*)&(depthInfos[0]), len);
		}
	}

	bool HeFrameInfo::Contains(const glm::vec3& point) const
	{
		return this->cameraInfo->GetFrustum()->Contains(point);
	}

	bool HeFrameInfo::ContainsAny(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const
	{
		return this->cameraInfo->GetFrustum()->ContainsAny(v0, v1, v2);
	}
	
	bool HeFrameInfo::ContainsAny(const vector<glm::vec3>& points) const
	{
		return this->cameraInfo->GetFrustum()->ContainsAny(points);
	}

	bool HeFrameInfo::ContainsAll(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const
	{
		return this->cameraInfo->GetFrustum()->ContainsAll(v0, v1, v2);
	}
	
	bool HeFrameInfo::ContainsAll(const vector<glm::vec3>& points) const
	{
		return this->cameraInfo->GetFrustum()->ContainsAll(points);
	}
}
