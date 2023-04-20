#include <Helium/Helium.h>

namespace ArtificialNature {

	Helium::Helium(const string& name, int windowWidth, int windowHeight)
		: HeObject(name)
	{
		graphics = new HeGraphics(name + ".Graphics", windowWidth, windowHeight);

		system("cd");

		std::ifstream i("Settings.json");
		if (i.is_open()) {
			i >> HeSettings;

			if (HeSettings.contains("Current Working Directory")) {
				auto path = HeSettings["Current Working Directory"].get<string>();
				std::filesystem::current_path(path);

				if (HeSettings.contains("Resource Root Directory"))
				{
					auto resourceRoot = HeSettings["Resource Root Directory"].get<string>();
					auto rr = (std::filesystem::current_path() / resourceRoot).string();
					replace(rr.begin(), rr.end(), '\\', '/');
					HeSettings["Resource Root Directory"] = rr;
				}
				else
				{
					if (std::filesystem::exists(std::filesystem::current_path() / "res"))
					{
						auto rr = (std::filesystem::current_path() / "res").string();
						replace(rr.begin(), rr.end(), '\\', '/');
						HeSettings["Resource Root Directory"] = rr;
					}
					else// if (std::filesystem::exists(std::filesystem::current_path() / "../../res"))
					{
						auto rr = (std::filesystem::current_path() / "../../res").string();
						replace(rr.begin(), rr.end(), '\\', '/');
						HeSettings["Resource Root Directory"] = rr;
					}
				}
			}
			else {
				auto path = std::filesystem::current_path();
				auto cwd = path.string();
				replace(cwd.begin(), cwd.end(), '\\', '/');
				HeSettings["Current Working Directory"] = cwd;

				if (std::filesystem::exists(std::filesystem::current_path() / "res"))
				{
					auto rr = (path / "res").string();
					replace(rr.begin(), rr.end(), '\\', '/');
					HeSettings["Resource Root Directory"] = rr;
				}
				else// if (std::filesystem::exists(std::filesystem::current_path() / "../../res"))
				{
					auto rr = (path / "../../res/").string();
					replace(rr.begin(), rr.end(), '\\', '/');
					HeSettings["Resource Root Directory"] = rr;
				}
			}
		}
		else {
			auto path = std::filesystem::current_path();
			auto cwd = path.string();
			replace(cwd.begin(), cwd.end(), '\\', '/');
			HeSettings["Current Working Directory"] = cwd;

			if (std::filesystem::exists(std::filesystem::current_path() / "res"))
			{
				auto rr = (path / "res").string();
				replace(rr.begin(), rr.end(), '\\', '/');
				HeSettings["Resource Root Directory"] = rr;
			}
			else// if (std::filesystem::exists(std::filesystem::current_path() / "../../res"))
			{
				auto rr = (path / "../../res/").string();
				replace(rr.begin(), rr.end(), '\\', '/');
				HeSettings["Resource Root Directory"] = rr;
			}

			std::ofstream o("Settings.json");
			o << std::setw(4) << HeSettings << std::endl;
		}
	}

	Helium::~Helium()
	{
		for (auto& kvp : scenes)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		scenes.clear();

		if (graphics != nullptr)
		{
			delete graphics;
			graphics = nullptr;
		}

		std::ofstream o("Settings.json");
		o << std::setw(4) << HeSettings << std::endl;
	}

	HeScene* Helium::GetScene(const string& sceneName)
	{
		if (scenes.count(sceneName) == 0)
		{
			scenes[sceneName] = new HeScene(sceneName, graphics);
		}

		return scenes[sceneName];
	}

	void Helium::InitializeImgui(GLFWwindow* window)
	{
		// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
	// GL ES 2.0 + GLSL 100
		const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
	// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
	// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}

	void Helium::TerminateImgui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Helium::Run()
	{
		if (callbackOnPrepare)
		{
			callbackOnPrepare();
		}

		while (finished == false)
		{
			if (callbackOnFrame)
			{
				callbackOnFrame();
			}
		}

		if (callbackOnTerminate)
		{
			callbackOnTerminate();
		}
	}
}
