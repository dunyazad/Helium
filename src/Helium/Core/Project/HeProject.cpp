#include <Helium/Core/Project/Project.h>
#include <Helium/Graphics/Graphics.h>

namespace ArtificialNature {

	HeProject::HeProject(const string& userName, const string& projectName)
	{
		this->userName = userName;
		this->projectName = projectName;
		this->rootPath = (filesystem::current_path() / "projects" / userName / projectName).string();

		FillOther();
	}

	HeProject::HeProject(const string& userName, const string& projectName, const string& projectsRootPath)
	{
		this->userName = userName;
		this->projectName = projectName;
		this->rootPath = projectsRootPath + "/projects/" + userName + "/" + projectName;

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
		this->cameraInfoPath = rootPath + "/camera_info";
		this->colorPath = rootPath + "/color";
		this->depthPath = rootPath + "/depth";
		this->resizedColorPath = rootPath + "/resized_color";
		this->resizedDepthPath = rootPath + "/resized_depth";
		this->convertedDepthPath = rootPath + "/converted_depth";
		this->roomPlanPath = rootPath + "/room_plan";
		this->reconstructedPath = rootPath + "/reconstructed";
		this->subVolumePath = rootPath + "/subvolumes";
		this->unwrapPath = rootPath + "/unwrap";
		this->resultPath = rootPath + "/result";

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
		CreateDirectoryIfNotExists(this->resultPath);

		json reconstruction_info;
		{
			auto reconstruction_info_file = reconstructedPath + "/reconstruction_info.json";
			if (filesystem::exists(reconstruction_info_file))
			{
				std::ifstream f(reconstruction_info_file);
				reconstruction_info = json::parse(f);
			}
		}
		{
			auto projectFile = rootPath + "/info.proj";
			std::ifstream f(projectFile);
			json info = json::parse(f);

			if (info.count("capturedFrameNumber") != 0)
			{
				int capturedFrameNumber = info["capturedFrameNumber"].get<int>();
				for (size_t i = 0; i <= capturedFrameNumber; i++)
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

	void HeProject::CreateDirectoryIfNotExists(const string& path)
	{
		if (filesystem::exists(path) == false)
		{
			filesystem::create_directories(path);
		}
	}

}
