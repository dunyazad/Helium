#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeGraphics;
	class HeImage;
	class HeFrustum;
	class HeCameraInfo;
	class HeFrameInfo;
	class HeProject;

	class HeFrameInfo
	{
	public:
		HeFrameInfo(HeProject* project, int frameIndex);
		HeFrameInfo(HeProject* project, int frameIndex, const json& reconstructionInfo);

		~HeFrameInfo();

		HeImage* LoadColorImage(HeGraphics* pGraphics);
		void LoadDepthInfo();

		inline int GetFrameIndex() const { return frameIndex; }
		inline const string& GetCameraInfoFile() const { return cameraInfoFile; }
		inline const string& GetDepthFile() const { return depthFile; }
		inline const string& GetResizedDepthFile() const { return resizedDepthFile; }
		inline const string& GetDepthDataFilePath() const { return depthDataFilePath; }
		inline const string& GetConvertedDepthFile() const { return convertedDepthFile; }
		inline const string& GetColorFile() const { return colorFile; }
		inline const string& GetResizedColorFile() const { return resizedColorFile; }
		inline const HeCameraInfo* GetCameraInfo() const { return cameraInfo; }
		inline HeImage* GetColorImage() const { return colorImage; }
		inline const vector<float>& GetDepthInfos() const { return depthInfos; }

	protected:
		HeProject* project;
		int frameIndex;
		string cameraInfoFile;
		string depthFile;
		string resizedDepthFile;
		string depthDataFilePath;
		string convertedDepthFile;
		string colorFile;
		string resizedColorFile;
		HeCameraInfo* cameraInfo;
		HeImage* colorImage;
		vector<float> depthInfos;
	};

}