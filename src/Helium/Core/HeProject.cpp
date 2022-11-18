#include <Helium/Core/HeProject.h>
#include <Helium/Graphics/Graphics.h>

namespace ArtificialNature {

	HeProject::HeProject(const string& userName, const string& projectName)
	{
		this->userName = userName;
		this->projectName = projectName;
		this->rootPath = ((filesystem::current_path() / "projects") / userName) / projectName;

		FillOther();
	}

	HeProject::HeProject(const string& userName, const string& projectName, const filesystem::path& projectsRootPath)
	{
		this->userName = userName;
		this->projectName = projectName;
		this->rootPath = ((projectsRootPath / "projects") / userName) / projectName;

		FillOther();
	}

	HeProject::~HeProject()
	{
		for (auto& frame : frames)
		{
			delete frame;
		}
		frames.clear();
	}

	void HeProject::FillOther()
	{
		this->cameraInfoPath = rootPath / "camera_info";
		this->colorPath = rootPath / "color";
		this->depthPath = rootPath / "depth";
		this->resizedColorPath = rootPath / "resized_color";
		this->resizedDepthPath = rootPath / "resized_depth";
		this->convertedDepthPath = rootPath / "converted_depth";
		this->roomPlanPath = rootPath / "room_plan";
		this->reconstructedPath = rootPath / "reconstructed";
		this->subVolumePath = rootPath / "subvolumes";
		this->unwrapPath = rootPath / "unwrap";

		CreateDirectoryIfNotExists(this->rootPath);
		CreateDirectoryIfNotExists(this->cameraInfoPath);
		CreateDirectoryIfNotExists(this->colorPath);
		CreateDirectoryIfNotExists(this->depthPath);
		CreateDirectoryIfNotExists(this->resizedColorPath);
		CreateDirectoryIfNotExists(this->resizedDepthPath);
		CreateDirectoryIfNotExists(this->convertedDepthPath);
		CreateDirectoryIfNotExists(this->roomPlanPath);
		CreateDirectoryIfNotExists(this->reconstructedPath);
		CreateDirectoryIfNotExists(this->subVolumePath);
		CreateDirectoryIfNotExists(this->unwrapPath);

		auto projectFile = rootPath / "info.proj";
		std::ifstream f(projectFile);
		json info = json::parse(f);

		if (info.count("capturedFrameNumber") != 0)
		{
			int capturedFrameNumber = info["capturedFrameNumber"].get<int>();
			for (size_t i = 0; i < capturedFrameNumber; i++)
			{
				this->frames.push_back(new HeFrameInfo(this, (int)i));
			}
		}
	}

	void HeProject::CreateDirectoryIfNotExists(const filesystem::path& path)
	{
		if (filesystem::exists(path) == false)
		{
			filesystem::create_directories(path);
		}
	}


	HeFrameInfo::HeFrameInfo(HeProject* project, int frameIndex)
	{
		this->project = project;
		this->frameIndex = frameIndex;
		this->cameraInfoFile = project->GetCameraInfoPath() / format("camera_info_{}.cam", frameIndex);
		this->depthFile = project->GetDepthPath() / format("depth_{}.png", frameIndex);
		this->resizedDepthFile = project->GetResizedDepthPath() / format("resized_depth_{}.png", frameIndex);
		this->depthDataFilePath = project->GetDepthPath() / format("depth_{}.dpt", frameIndex);
		this->convertedDepthFile = project->GetConvertedDepthPath() / format("depth_{}.png", frameIndex);
		this->colorFile = project->GetColorPath() / format("color_{}.jpg", frameIndex);
		this->resizedColorFile = project->GetResizedColorPath() / format("resized_color_{}.jpg", frameIndex);
		this->cameraInfo = new HeCameraInfo(this, this->cameraInfoFile);
	}

	HeFrameInfo::~HeFrameInfo()
	{
		delete this->cameraInfo;
	}

	void HeFrameInfo::LoadColorImage(HeGraphics* pGraphics)
	{
		stringstream ss;
		ss << "frame" << frameIndex;
		colorImage = pGraphics->GetImage(ss.str(), colorFile.string());
	}

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const filesystem::path& cameraInfoFile)
	{
		string cameraInfoFileString = cameraInfoFile.string();

		FILE* fp = nullptr;
		fopen_s(&fp, cameraInfoFileString.c_str(), "rb");

		if (fp != nullptr)
		{
			auto size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[size];
			fread(buffer, size, 1, fp);
			fclose(fp);

			int index = 0;
			int frame_index = 0; memcpy(&frame_index, buffer + index, 4); index += 4;
			int image_width = 0; memcpy(&image_width, buffer + index, 4); index += 4;
			int image_height = 0; memcpy(&image_height, buffer + index, 4); index += 4;
			float fx = 0; memcpy(&fx, buffer + index, 4); index += 4;
			float fy = 0; memcpy(&fy, buffer + index, 4); index += 4;
			float ox = 0; memcpy(&ox, buffer + index, 4); index += 4;
			float oy = 0; memcpy(&oy, buffer + index, 4); index += 4;

			float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
			float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
			float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
			float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;

			delete buffer;

			this->viewMatrix = glm::make_mat4(view_matrix);

			float flip[16] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1 };
			auto flipMatrix = glm::make_mat4(flip);

			glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
			intrinsic_matrix[2][2] = 1.0f;
			this->intrinsicMatrix = intrinsic_matrix;
			this->fx = this->intrinsicMatrix[0][0];
			this->fy = this->intrinsicMatrix[1][1];
			this->ox = this->intrinsicMatrix[0][2];
			this->oy = this->intrinsicMatrix[1][2];

			this->viewMatrixInversed = glm::inverse(this->viewMatrix);

			this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

			this->extrinsicMatrix = this->localToWorldMatrix;

			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(this->transformMatrix, scale, rotation, translation, skew, perspective);

			this->position = translation;
			this->rotation = glm::toMat3(rotation);

			this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}
	}

	HeCameraInfo::~HeCameraInfo()
	{
		delete this->frustum;
	}

}