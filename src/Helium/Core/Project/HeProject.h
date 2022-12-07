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

		HeProject(const string& userName, const string& projectName, const string& projectsRootPath);

		~HeProject();

		void FillOther();

		void CreateDirectoryIfNotExists(const string& path);

		inline const string& GetRootPath() const { return this->rootPath; }
		inline const string& GetCameraInfoPath() const { return this->cameraInfoPath; }
		inline const string& GetColorPath() const { return this->colorPath; }
		inline const string& GetDepthPath() const { return this->depthPath; }
		inline const string& GetResizedColorPath() const { return this->resizedColorPath; }
		inline const string& GetResizedDepthPath() const { return this->resizedDepthPath; }
		inline const string& GetConvertedDepthPath() const { return this->convertedDepthPath; }
		inline const string& GetRoomPlanPath() const { return this->roomPlanPath; }
		inline const string& GetReconstructedPath() const { return this->reconstructedPath; }
		inline const string& GetSubVolumePath() const { return this->subVolumePath; }
		inline const string& GetUnwrapPath() const { return this->unwrapPath; }
		inline const string& GetResultPath() const { return this->resultPath; }
		inline const vector<HeFrameInfo*>& GetFrames() const { return this->frames; }

	protected:
		string userName;
		string projectName;
		string rootPath;
		string cameraInfoPath;
		string colorPath;
		string depthPath;
		string resizedColorPath;
		string resizedDepthPath;
		string convertedDepthPath;
		string roomPlanPath;
		string reconstructedPath;
		string subVolumePath;
		string unwrapPath;
		string resultPath;

		vector<HeFrameInfo*> frames;
	};

}
