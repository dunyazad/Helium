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

		json reconstruction_info;
		{
			auto reconstruction_info_file = reconstructedPath / "reconstruction_info.json";
			if (filesystem::exists(reconstruction_info_file))
			{
				std::ifstream f(reconstruction_info_file);
				reconstruction_info = json::parse(f);
			}
		}
		{
			auto projectFile = rootPath / "info.proj";
			std::ifstream f(projectFile);
			json info = json::parse(f);

			if (info.count("capturedFrameNumber") != 0)
			{
				int capturedFrameNumber = info["capturedFrameNumber"].get<int>();
				for (size_t i = 0; i < capturedFrameNumber; i++)
				{
					if (reconstruction_info.empty()) {
						this->frames.push_back(new HeFrameInfo(this, (int)i));
					}
					else {
						this->frames.push_back(new HeFrameInfo(this, (int)i, reconstruction_info));
					}
				}
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

	HeFrameInfo::HeFrameInfo(HeProject* project, int frameIndex, const json& reconstructionInfo)
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
		this->cameraInfo = new HeCameraInfo(this, this->cameraInfoFile, reconstructionInfo);

		LoadDepthInfo();
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

	void HeFrameInfo::LoadDepthInfo()
	{
		auto depthPath = project->GetDepthPath();

		HeFile file;
		stringstream ss;
		ss << "depth_" << frameIndex << ".dpt";
		if (file.Open((depthPath / ss.str()).string(), true)) {
			auto len = file.GetFileLength();
			depthInfos.resize(len / 4);
			file.Read((char*)&(depthInfos[0]), len);
		}
	}

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const filesystem::path& cameraInfoFile)
	{
		string cameraInfoFileString = cameraInfoFile.string();

		HeFile file;
		if (file.Open(cameraInfoFileString, true))
		{
			int size = file.GetFileLength();
			char* buffer = new char[size];
			file.Read(buffer, size);
			file.Close();

			int index = 0;
			memcpy(&this->frameIndex, buffer + index, 4); index += 4;
			memcpy(&this->imageWidth, buffer + index, 4); index += 4;
			memcpy(&this->imageHeight, buffer + index, 4); index += 4;

			memcpy(&this->fx, buffer + index, 4); index += 4;
			memcpy(&this->fy, buffer + index, 4); index += 4;
			memcpy(&this->ox, buffer + index, 4); index += 4;
			memcpy(&this->oy, buffer + index, 4); index += 4;

			float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
			float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
			float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
			float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;
			float projection_matrix[16]; memcpy(&projection_matrix, buffer + index, 64); index += 64;
			float view_projection_matrix[16]; memcpy(&view_projection_matrix, buffer + index, 64); index += 64;

			delete[] buffer;

			glm::mat4 tm = glm::make_mat4(transform_matrix);
			auto p = glm::vec3(glm::row(tm, 3));
			auto r = glm::mat3(glm::transpose(tm)) * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			this->transformMatrix = glm::translate(glm::mat4(r), p);
			this->transformMatrix = glm::transpose(this->transformMatrix);

			this->viewMatrix = glm::make_mat4(view_matrix);
			this->viewMatrixInversed = glm::inverse(this->viewMatrix);

			//this->viewMatrix = glm::transpose(glm::make_mat4(view_matrix));

			glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
			intrinsic_matrix[2][2] = 1.0f;

			this->intrinsicMatrix = intrinsic_matrix;
			this->fx = this->intrinsicMatrix[0][0];
			this->fy = this->intrinsicMatrix[1][1];
			this->ox = this->intrinsicMatrix[0][2];
			this->oy = this->intrinsicMatrix[1][2];


			this->localToWorldMatrix = glm::inverse(glm::flip_axes(this->viewMatrixInversed));

			//this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

			this->extrinsicMatrix = this->localToWorldMatrix;

			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			this->projectionMatrix = glm::transpose(glm::make_mat4(projection_matrix));

			this->viewProjectionMatrix = glm::make_mat4(view_projection_matrix);

			this->position = p;
			this->rotation = r;

			auto r0 = glm::row(r, 0);
			auto r1 = -glm::row(r, 1);
			auto r2 = -glm::row(r, 2);

			glm::mat3 rr;
			rr[0] = r0;
			rr[1] = r1;
			rr[2] = r2;

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}
	}

	HeCameraInfo::HeCameraInfo(HeFrameInfo* frameInfo, const filesystem::path& cameraInfoFile, const json& reconstructionInfo)
	{
		string cameraInfoFileString = cameraInfoFile.string();

		HeFile file;
		if (file.Open(cameraInfoFileString, true))
		{
			int size = file.GetFileLength();
			char* buffer = new char[size];
			file.Read(buffer, size);
			file.Close();

			int index = 0;
			memcpy(&this->frameIndex, buffer + index, 4); index += 4;
			memcpy(&this->imageWidth, buffer + index, 4); index += 4;
			memcpy(&this->imageHeight, buffer + index, 4); index += 4;

			memcpy(&this->fx, buffer + index, 4); index += 4;
			memcpy(&this->fy, buffer + index, 4); index += 4;
			memcpy(&this->ox, buffer + index, 4); index += 4;
			memcpy(&this->oy, buffer + index, 4); index += 4;

			float intrinsics[9]; memcpy(&intrinsics, buffer + index, 36); index += 36;
			float view_matrix[16]; memcpy(&view_matrix, buffer + index, 64); index += 64;
			float local_to_world_matrix[16]; memcpy(&local_to_world_matrix, buffer + index, 64); index += 64;
			float transform_matrix[16]; memcpy(&transform_matrix, buffer + index, 64); index += 64;
			float projection_matrix[16]; memcpy(&projection_matrix, buffer + index, 64); index += 64;
			float view_projection_matrix[16]; memcpy(&view_projection_matrix, buffer + index, 64); index += 64;

			delete[] buffer;

			glm::mat4 tm = glm::make_mat4(transform_matrix);
			auto p = glm::vec3(glm::row(tm, 3));
			auto r = glm::mat3(glm::transpose(tm)) * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			this->transformMatrix = glm::mat4(r);
			this->transformMatrix[3] = glm::vec4(p, 1.0f);
			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			this->viewMatrix = glm::make_mat4(view_matrix);
			this->viewMatrixInversed = glm::inverse(this->viewMatrix);

			//this->viewMatrix = glm::transpose(glm::make_mat4(view_matrix));

			glm::mat3 intrinsic_matrix = glm::make_mat3(intrinsics) / 7.5f;
			intrinsic_matrix[2][2] = 1.0f;

			this->intrinsicMatrix = intrinsic_matrix;
			this->fx = this->intrinsicMatrix[0][0];
			this->fy = this->intrinsicMatrix[1][1];
			this->ox = this->intrinsicMatrix[0][2];
			this->oy = this->intrinsicMatrix[1][2];


			this->localToWorldMatrix = glm::inverse(glm::flip_axes(this->viewMatrixInversed));

			//this->localToWorldMatrix = glm::inverse(this->viewMatrixInversed * flipMatrix);

			this->extrinsicMatrix = this->localToWorldMatrix;

			this->projectionMatrix = glm::transpose(glm::make_mat4(projection_matrix));

			this->viewProjectionMatrix = glm::make_mat4(view_projection_matrix);

			this->position = p;
			this->rotation = r;

			auto r0 = glm::row(r, 0);
			auto r1 = -glm::row(r, 1);
			auto r2 = -glm::row(r, 2);

			glm::mat3 rr;
			rr[0] = r0;
			rr[1] = r1;
			rr[2] = r2;

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}

		{
			auto& info = reconstructionInfo["frames"][frameInfo->GetFrameIndex()];
			this->imageWidth = info["image_width"].get<int>();
			this->imageHeight = info["image_height"].get<int>();
			this->fx = info["fx"].get<float>();
			this->fy = info["fy"].get<float>();
			this->ox = info["ox"].get<float>();
			this->oy = info["oy"].get<float>();
			this->intrinsicMatrix = glm::make_mat4(info["intrinsic_matrix"].get<vector<float>>().data());
			this->extrinsicMatrix = glm::make_mat4(info["extrinsic_matrix"].get<vector<float>>().data());
			this->position = glm::vec3(glm::row(glm::inverse(this->extrinsicMatrix), 3));
			this->rotation = glm::mat3(this->extrinsicMatrix);

			this->transformMatrix = glm::mat4(this->rotation);
			this->transformMatrix[3] = glm::vec4(this->position, 1.0f);
			this->transformMatrixInversed = glm::inverse(this->transformMatrix);

			this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);

			auto rr = this->rotation * glm::mat3(glm::angleAxis(glm::radians(180.0f), glm::vec3(1, 0, 0)));
			rr[2] = -rr[2];

			//this->frustum = new HeFrustum(this->position, this->rotation, this->imageWidth, this->imageHeight, this->fx, this->fy);
			this->frustum = new HeFrustum(this->position, rr, this->imageWidth, this->imageHeight, this->fx, this->fy);
		}
	}

	HeCameraInfo::~HeCameraInfo()
	{
		delete this->frustum;
	}

	glm::vec3 RayPlaneIntersection(const glm::vec3& planePoint, const glm::vec3& planeNormal, const glm::vec3& rayPoint, const glm::vec3& rayDirection)
	{
		auto ndotu = glm::dot(planeNormal, rayDirection);

		if (abs(ndotu) < FLT_EPSILON) {
			cout << "No intersection or line is within plane." << endl;
		}

		auto w = rayPoint - planePoint;
		auto si = -glm::dot(planeNormal, w) / ndotu;
		return w + si * rayDirection + planePoint;
	}

	glm::vec2 HeCameraInfo::WorldToUV(const glm::vec3& worldPosition) const
	{
		//auto pp = this->viewProjectionMatrix * glm::vec4(worldPosition, 1.0f);

		auto ip4 = this->transformMatrixInversed * glm::vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1);
		//auto uv4 = this->projectionMatrix * this->transformMatrixInversed * glm::vec4(worldPosition.x, worldPosition.y, worldPosition.z, 1);
		glm::vec3 planePoint = glm::vec3(0, 0, this->fx);
		glm::vec3 planeNormal = glm::vec3(0, 0, 1);
		glm::vec3 rayPoint = glm::vec3(0, 0, 0);
		glm::vec3 rayDirection = glm::normalize(glm::vec3(ip4.x, ip4.y, ip4.z));
		glm::vec3 intersection = RayPlaneIntersection(planePoint, planeNormal, rayPoint, rayDirection);
		float u = ((intersection.x / (this->imageWidth * 0.5f)) * 0.5f + 0.5f);
		float v = 1 - ((intersection.y / (this->imageHeight * 0.5f)) * 0.5f + 0.5f);
		return glm::vec2(u, v);
	}
}
