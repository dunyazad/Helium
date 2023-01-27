#define USING_FILES_SYSTEM
#include <Helium/Helium.h>
using namespace ArtificialNature;

int windowWidth = 1024;
int windowHeight = 1024;

float controlValue = 0.5f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);

HeGraphics* gGraphics = nullptr;
HeScene* gScene = nullptr;
//HeOrthogonalCamera* pCamera = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorFlight* pCameraManipulator = nullptr;
//HeCameraManipulatorOrtho* pCameraManipulator = nullptr;

HeGeometryThickLines* pDebugGeometry = nullptr;

int capturedFrameCount = 0;
int selectedFrame = -1;
vector<HeFrameInfo*> frameInformations;
HeProject* project = nullptr;

class OnOff {
public:
	void AddSceneNode(HeSceneNode* node) {
		nodes.push_back(node);
	}

	void First()
	{
		nodes[index]->SetActive(false);
		nodes[0]->SetActive(true);
		index = 0;
	}

	void Last()
	{
		nodes[index]->SetActive(false);
		nodes[nodes.size() - 1]->SetActive(true);
		index = nodes.size() - 1;
	}

	void Next()
	{
		if (index + 1 < nodes.size()) {
			HideAll();

			nodes[index]->SetActive(false);
			nodes[index + 1]->SetActive(true);
			index++;
		}
	}

	void Previous()
	{
		if (index > 0) {
			HideAll();

			nodes[index]->SetActive(false);
			nodes[index - 1]->SetActive(true);
			index--;
		}
	}

	void On(int index)
	{
		if (-1 < index && index < nodes.size())
		{
			HideAll();

			nodes[index]->SetActive(true);
		}
	}

	void HideAll()
	{
		for (auto& n : nodes)
		{
			n->SetActive(false);
		}
	}

protected:
	vector<HeSceneNode*> nodes;
	size_t index = 0;
};
OnOff onoff;

class HeVolume
{
public:
	HeVolume(const glm::vec3& vmin, const glm::vec3& vmax, float voxelSize = 0.05f) :
		aabb(vmin, vmax), voxelSize(voxelSize)
	{
		xCount = int(floorf((vmax.x - vmin.x) / voxelSize)) + 1;
		yCount = int(floorf((vmax.x - vmin.x) / voxelSize)) + 1;
		zCount = int(floorf((vmax.x - vmin.x) / voxelSize)) + 1;

		voxels = new bool[xCount * yCount * zCount];
	}

	~HeVolume()
	{
		HeDelete(voxels);
	}

	glm::ivec3 GetIndex(const glm::vec3& position)
	{
		int xIndex = floorf(position.x / voxelSize);
		int yIndex = floorf(position.y / voxelSize);
		int zIndex = floorf(position.z / voxelSize);
		return glm::ivec3(xIndex, yIndex, zIndex);
	}

	bool& At(int x, int y, int z)
	{
		return voxels[z * (yCount * xCount) + y * (xCount) + x];
	}

	bool& At(glm::ivec3& index)
	{
		return voxels[index.z * (yCount * xCount) + index.y * (xCount) + index.x];
	}

	void Iterate(function<void(const glm::vec3&, const glm::vec3&, bool)> callback)
	{
		for (int z = 0; z < zCount; z++)
		{
			for (int y = 0; y < yCount; y++)
			{
				for (int x = 0; x < xCount; x++)
				{
					auto vmin = aabb.GetMin() + glm::vec3(voxelSize * x, voxelSize * y, voxelSize * z);
					auto vmax = aabb.GetMin() + glm::vec3(voxelSize * (x + 1), voxelSize * (y + 1), voxelSize * (z + 1));

					callback(vmin, vmax, At(x, y, z));
				}
			}
		}
	}

protected:
	HeAABB aabb;
	int xCount = 0;
	int yCount = 0;
	int zCount = 0;
	bool* voxels = nullptr;
	float voxelSize = 0.05f;
};

class HeVisualDebugger
{
public:
	HeVisualDebugger(Helium& helium)
		: helium(helium)
	{
		scene = helium.GetScene("Default Scene");
		graphics = helium.GetGraphics();

		solidSceneNode = scene->CreateSceneNode("Visual Debugger.SolidSceneNode");
		solidGeometry = graphics->GetGeometryTriangleSoup("Visual Debugger.SolidGeometry");
		solidGeometry->Initialize();
		solidSceneNode->AddGeometry(solidGeometry);
		solidMaterial = gGraphics->GetMaterial("Visual Debugger.SolidMaterial");
		solidShader = gGraphics->GetShader("Visual Debugger.SolidShader", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
		solidMaterial->SetShader(solidShader);
		solidGeometry->SetMaterial(solidMaterial);

		lineSceneNode = scene->CreateSceneNode("Visual Debugger.LineSceneNode");
		lineGeometry = graphics->GetGeometryThickLines("Visual Debugger.LineGeometry");
		lineGeometry->Initialize();
		lineSceneNode->AddGeometry(lineGeometry);
		lineMaterial = gGraphics->GetMaterial("Visual Debugger.LineMaterial");
		lineShader = gGraphics->GetShader("Visual Debugger.LineShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
		lineMaterial->SetShader(lineShader);
		lineGeometry->SetMaterial(lineMaterial);
		lineGeometry->SetThickness(1);
		lineGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

		axisSceneNode = scene->CreateSceneNode("Visual Debugger.AxisSceneNode");
		axisGeometry = graphics->GetGeometryThickLines("Visual Debugger.AxisGeometry");
		axisGeometry->Initialize();
		axisSceneNode->AddGeometry(axisGeometry);
		axisMaterial = gGraphics->GetMaterial("Visual Debugger.AxisMaterial");
		axisShader = gGraphics->GetShader("Visual Debugger.AxisShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
		axisMaterial->SetShader(axisShader);
		axisGeometry->SetMaterial(axisMaterial);
		axisGeometry->SetThickness(1);
		axisGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

		axisGeometry->AddVertex(glm::vec3(-1024, 0, 0));
		axisGeometry->AddVertex(glm::vec3(1024, 0, 0));
		axisGeometry->AddVertex(glm::vec3(0, -1024, 0));
		axisGeometry->AddVertex(glm::vec3(0, 1024, 0));
		axisGeometry->AddVertex(glm::vec3(0, 0, -1024));
		axisGeometry->AddVertex(glm::vec3(0, 0, 1024));

		axisGeometry->AddColor(glm::vec4(1, 0, 0, 1));
		axisGeometry->AddColor(glm::vec4(1, 0, 0, 1));
		axisGeometry->AddColor(glm::vec4(0, 1, 0, 1));
		axisGeometry->AddColor(glm::vec4(0, 1, 0, 1));
		axisGeometry->AddColor(glm::vec4(0, 0, 1, 1));
		axisGeometry->AddColor(glm::vec4(0, 0, 1, 1));

		//{
		//	auto pNode = gScene->CreateSceneNode("Grid");
		//	auto pGeometry = gGraphics->GetGeometryPlane("Grid.Geometry", 2, 2, 1, 1, HePlaneType::XY);
		//	pGeometry->Initialize();
		//	pNode->AddGeometry(pGeometry);
		//	auto pMaterial = gGraphics->GetMaterial("Grid.Material");
		//	pGeometry->SetMaterial(pMaterial);
		//	auto pShader = gGraphics->GetShader("Grid.Shader", "../../res/shader/grid.vs", "../../res/shader/grid.fs");
		//	pMaterial->SetShader(pShader);
		//}
	}

	void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		AddTriangle(v0, v1, v2, glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1));
	}

	void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color0, const glm::vec4& color1, const glm::vec4& color2)
	{
		int vertexCount = (int)lineGeometry->GetVertexCount();

		lineGeometry->AddVertex(v0);
		lineGeometry->AddVertex(v1);
		lineGeometry->AddVertex(v1);
		lineGeometry->AddVertex(v2);
		lineGeometry->AddVertex(v2);
		lineGeometry->AddVertex(v0);

		lineGeometry->AddIndex(vertexCount);
		lineGeometry->AddIndex(vertexCount + 1);
		lineGeometry->AddIndex(vertexCount + 2);
		lineGeometry->AddIndex(vertexCount + 3);
		lineGeometry->AddIndex(vertexCount + 4);
		lineGeometry->AddIndex(vertexCount + 5);

		lineGeometry->AddColor(color0);
		lineGeometry->AddColor(color1);
		lineGeometry->AddColor(color1);
		lineGeometry->AddColor(color2);
		lineGeometry->AddColor(color2);
		lineGeometry->AddColor(color0);

		solidGeometry->AddTriangle(v0, v1, v2, color0, color1, color2);
	}

	void AddLine(const glm::vec3& v0, const glm::vec3& v1)
	{
		AddLine(v0, v1, glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1));
	}

	void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& color0, const glm::vec4& color1)
	{
		int vertexCount = (int)lineGeometry->GetVertexCount();

		lineGeometry->AddVertex(v0);
		lineGeometry->AddVertex(v1);

		lineGeometry->AddIndex(vertexCount);
		lineGeometry->AddIndex(vertexCount + 1);

		lineGeometry->AddColor(color0);
		lineGeometry->AddColor(color1);
	}

	void AddBox(const glm::vec3& bmin, const glm::vec3& bmax)
	{
		AddBox(bmin, bmax, glm::vec4(1, 1, 1, 1));
	}

	void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const glm::vec4& color)
	{
		glm::vec3 llb(bmin.x, bmin.y, bmin.z);
		glm::vec3 rlb(bmax.x, bmin.y, bmin.z);
		glm::vec3 lub(bmin.x, bmax.y, bmin.z);
		glm::vec3 rub(bmax.x, bmax.y, bmin.z);
		glm::vec3 llf(bmin.x, bmin.y, bmax.z);
		glm::vec3 rlf(bmax.x, bmin.y, bmax.z);
		glm::vec3 luf(bmin.x, bmax.y, bmax.z);
		glm::vec3 ruf(bmax.x, bmax.y, bmax.z);

		int vertexCount = (int)lineGeometry->GetVertexCount();

		lineGeometry->AddVertex(llf);
		lineGeometry->AddVertex(rlf);
		lineGeometry->AddVertex(llb);
		lineGeometry->AddVertex(rlb);
		lineGeometry->AddVertex(llf);
		lineGeometry->AddVertex(llb);
		lineGeometry->AddVertex(rlf);
		lineGeometry->AddVertex(rlb);

		lineGeometry->AddVertex(luf);
		lineGeometry->AddVertex(ruf);
		lineGeometry->AddVertex(lub);
		lineGeometry->AddVertex(rub);
		lineGeometry->AddVertex(luf);
		lineGeometry->AddVertex(lub);
		lineGeometry->AddVertex(ruf);
		lineGeometry->AddVertex(rub);

		lineGeometry->AddVertex(llf);
		lineGeometry->AddVertex(luf);
		lineGeometry->AddVertex(rlf);
		lineGeometry->AddVertex(ruf);
		lineGeometry->AddVertex(llb);
		lineGeometry->AddVertex(lub);
		lineGeometry->AddVertex(rlb);
		lineGeometry->AddVertex(rub);

		for (int i = vertexCount; i < (int)lineGeometry->GetVertexCount(); i++)
		{
			lineGeometry->AddIndex(i);
			lineGeometry->AddColor(color);
		}
	}

protected:
	Helium& helium;
	HeScene* scene = nullptr;
	HeGraphics* graphics = nullptr;

	HeSceneNode* solidSceneNode = nullptr;
	HeGeometryTriangleSoup* solidGeometry = nullptr;
	HeMaterial* solidMaterial = nullptr;
	HeShader* solidShader = nullptr;

	HeSceneNode* lineSceneNode = nullptr;
	HeGeometryThickLines* lineGeometry = nullptr;
	HeMaterial* lineMaterial = nullptr;
	HeShader* lineShader = nullptr;

	HeSceneNode* axisSceneNode = nullptr;
	HeGeometryThickLines* axisGeometry = nullptr;
	HeMaterial* axisMaterial = nullptr;
	HeShader* axisShader = nullptr;
};

HeVisualDebugger* vd = nullptr;

int main(int argc, char** argv)
{
	// Load GLFW and Create a Window
	glfwInit(); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Window Visibility
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // Transparent Background
	auto mWindow = glfwCreateWindow(windowWidth, windowHeight, "AppHelium", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	//glfwSetWindowAttrib(mWindow, GLFW_DECORATED, GLFW_FALSE); // No Window Title Bar

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1); // Enable vsync
	//glfwSwapInterval(0); // Disable vsync

	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, mouse_position_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	glfwSetScrollCallback(mWindow, mouse_wheel_callback);

	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	glEnable(GL_MULTISAMPLE);

	Helium helium("helium", windowWidth, windowHeight);
	helium.InitializeImgui(mWindow);

	gGraphics = helium.GetGraphics();
	gScene = helium.GetScene("Default Scene");

	helium.OnPrepare([&]() {
		cout << "Helium::OnPrepare" << endl;

		pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
		//pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
		pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
		gScene->SetMainCamera(pCamera);
		pCameraManipulator = gScene->CreateCameraManipulatoFlight("Main Camera Manipulator", pCamera);

		auto visualDebugger = HeVisualDebugger(helium);
		vd = &visualDebugger;

		{
			auto pNode = gScene->CreateSceneNodeImgui("imgui");
			pNode->SetText("0");
		}

		HeVolume volume(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		//volume.integrate(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f));

		//visualDebugger.AddTriangle(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f));

		//visualDebugger.AddTriangle(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f));

		//HeTime timer("AddBox");
		volume.Iterate([&](const glm::vec3& vmin, const glm::vec3& vmax, bool value) {
			if (value)
			{
				//vd->AddBox(vmin, vmax, glm::vec4(1, 0, 0, 1));
			}
			else
			{
				//vd->AddBox(vmin, vmax, glm::vec4(1, 1, 1, 0.01));
			}
			//timer.Touch();
		});
		//timer.Stop();

		//HeTime timer1("AddBox1");
		//vd->AddBox(glm::vec3(-0.25f, -0.25f, -0.25f), glm::vec3(0.25f, 0.25f, 0.25f));
		//timer1.Stop();



		{
			project = new HeProject("default", "data", "D:\\Workspace\\Reconstruct");
			capturedFrameCount = project->GetFrames().size();

			for (size_t i = 0; i < capturedFrameCount; i++)
			{
				auto frame = project->GetFrames()[i];
				auto cameraInfo = frame->GetCameraInfo();
				auto frustum = cameraInfo->GetFrustum();
				auto& nr = frustum->GetNormalizedRight();
				auto& nu = frustum->GetNormalizedUp();
				auto& nf = frustum->GetNormalizedFront();
				auto& fp = frustum->GetPosition();
				const auto& m = cameraInfo->GetTransformMatrix();

				auto right = glm::vec3(m * glm::vec4(1, 0, 0, 0));
				auto up = glm::vec3(m * glm::vec4(0, 1, 0, 0));
				auto front = glm::vec3(m * glm::vec4(0, 0, 1, 0));
				vd->AddLine(fp, fp + right * 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 0, 0, 1));
				vd->AddLine(fp, fp + up * 0.1f, glm::vec4(0, 1, 0, 1), glm::vec4(0, 1, 0, 1));
				vd->AddLine(fp, fp + front * 0.1f, glm::vec4(0, 0, 1, 1), glm::vec4(0, 0, 1, 1));

				auto pNode = gScene->CreateSceneNode(format("Frame Color Image {}", i));
				auto pGeometry = gGraphics->GetGeometry(format("Frame Color Image Geometry {}", i));
				pGeometry->AddVertex(glm::vec3(-0.096, -0.072, 0.01f));
				pGeometry->AddVertex(glm::vec3( 0.096, -0.072, 0.01f));
				pGeometry->AddVertex(glm::vec3(-0.096,  0.072, 0.01f));
				pGeometry->AddVertex(glm::vec3( 0.096,  0.072, 0.01f));
				pGeometry->AddUV(glm::vec2(0.0, 0.0));
				pGeometry->AddUV(glm::vec2(1.0, 0.0));
				pGeometry->AddUV(glm::vec2(0.0, 1.0));
				pGeometry->AddUV(glm::vec2(1.0, 1.0));
				pGeometry->AddIndex(0);
				pGeometry->AddIndex(1);
				pGeometry->AddIndex(2);
				pGeometry->AddIndex(2);
				pGeometry->AddIndex(1);
				pGeometry->AddIndex(3);
				pGeometry->Initialize();
				pNode->AddGeometry(pGeometry);
				pNode->SetLocalPosition(fp);
				pNode->SetLocalRotation(glm::quat_cast(m));

				onoff.AddSceneNode(pNode);

				auto pMaterial = gGraphics->GetMaterialSingleTexture(format("Frame Material {}", i));
				pGeometry->SetMaterial(pMaterial);
				auto pShader = gGraphics->GetShader(format("Frame Color Image Shader {}", i), "../../res/shader/texture.vs", "../../res/shader/texture.fs");
				pMaterial->SetShader(pShader);

				auto pImage = frame->LoadColorImage(gGraphics);
				pImage->Initialize();
				auto pTexture = gGraphics->GetTexture(format("Frame Color Image Texture {}", i), pImage);
				pTexture->Initialize();
				pMaterial->SetTexture(pTexture);

				auto pLineGeometry = gGraphics->GetGeometryThickLines("lines");
				pLineGeometry->Initialize();
				pNode->AddGeometry(pLineGeometry);
				auto pLineMaterial = gGraphics->GetMaterial("Visual Debugger.LineMaterial");
				auto pLineShader = gGraphics->GetShader("Visual Debugger.LineShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
				pLineMaterial->SetShader(pLineShader);
				pLineGeometry->SetMaterial(pLineMaterial);
				pLineGeometry->SetThickness(1);
				pLineGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

				pLineGeometry->AddVertex(glm::vec3(0, 0, 0));
				pLineGeometry->AddVertex(glm::vec3(0.05f, 0, 0));
				pLineGeometry->AddVertex(glm::vec3(0, 0, 0));
				pLineGeometry->AddVertex(glm::vec3(0, 0.05f, 0));
				pLineGeometry->AddVertex(glm::vec3(0, 0, 0));
				pLineGeometry->AddVertex(glm::vec3(0, 0, 0.05f));

				pLineGeometry->AddColor(glm::vec4(0, 1, 1, 1));
				pLineGeometry->AddColor(glm::vec4(0, 1, 1, 1));
				pLineGeometry->AddColor(glm::vec4(1, 0, 1, 1));
				pLineGeometry->AddColor(glm::vec4(1, 0, 1, 1));
				pLineGeometry->AddColor(glm::vec4(1, 1, 0, 1));
				pLineGeometry->AddColor(glm::vec4(1, 1, 0, 1));
				pLineGeometry->AddIndex(0);
				pLineGeometry->AddIndex(1);
				pLineGeometry->AddIndex(2);
				pLineGeometry->AddIndex(3);
				pLineGeometry->AddIndex(4);
				pLineGeometry->AddIndex(5);
			}
		}

		});



	int cnt = 0;

	auto lastTime = HeTime::Now();
	double accTime = 0.0;
	int frameCount = 0;
	int fps = 0;
	int incremental = 0;

	helium.OnFrame([&]() {
		auto delta = HeTime::DeltaMili(lastTime);
		lastTime = HeTime::Now();

		accTime += delta;
		frameCount++;
		if (accTime >= 1000.0)
		{
			accTime -= 1000.0;
			fps = frameCount;
			frameCount = 0;
		}

		glDepthFunc(GL_LEQUAL);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		//glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//if(cnt == 20)
		//{
		//    auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		//    //auto pMaterial = dynamic_cast<HeMaterialTextureArray*>(gGraphics->GetMaterial("texture array plane"));
		//    pMaterial->SetIncremental(incremental);
		//    onoff.On(incremental);

		//    incremental++;
		//    if (incremental > capturedFrameCount) {
		//        incremental = 0;
		//    }

		//    cnt = 0;
		//}
		//cnt++;

		gScene->Update((float)delta);
		gScene->Render();

		gGraphics->Flush();

		{
			auto pNode = gScene->GetSceneNodeIMGUI();
			pNode->SetText(format("{}", frameCount));
		}

		gScene->UpdateImgui((float)delta);
		gScene->RenderImgui();

		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		helium.SetFinished(glfwWindowShouldClose(mWindow));
		});

	helium.Run();

	helium.OnTerminate([&]() {
		helium.TerminateImgui();

		glfwTerminate();
		});

	return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);

	pCamera->SetAspectRatio((float)windowWidth / (float)windowHeight);

	//gGraphics->GetFrameBuffer("FrameBuffer")->Resize(windowWidth, height);

	//auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
	//if (pMaterial)
	//{
	//	auto pShader = pMaterial->GetShader();
	//	pShader->Use();
	//	pShader->SetUniformInt("screenWidth", windowWidth);
	//	pShader->SetUniformInt("screenHeight", windowHeight);
	//}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnKey(window, key, scancode, action, mods);
	}

	if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		//auto image = gGraphics->GetCanvasImage("Capture", mWidth, mHeight);
		//image->CaptureFrame("capture.png");

		vector<string> fileNames;
		for (size_t i = 0; i < 1; i++)
		{
			stringstream ss;
			ss << "texture_" << i << ".png";
			fileNames.push_back(ss.str());
		}

		gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
			auto pNode = dynamic_cast<HeSceneNodeImgui*>(gScene->GetSceneNode("imgui"));
			if (pNode != nullptr) {
				pNode->SetText(format("{}", frameNumber + 1));
			}
			//pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
			});
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		onoff.Previous();
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		onoff.Next();
	}
	else if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		controlValue += 0.01;

		controlValue = controlValue > 0.0 ? controlValue : 0.0;
		controlValue = controlValue < 1.0 ? controlValue : 1.0;

		cout << "Control Value : " << controlValue << endl;

		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			auto pShader = pMaterial->GetShader();
			pShader->Use();
			pShader->SetUniformFloat("controlValue", controlValue);
		}
	}
	else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		controlValue -= 0.01;

		controlValue = controlValue > 0.0 ? controlValue : 0.0;
		controlValue = controlValue < 1.0 ? controlValue : 1.0;

		cout << "Control Value : " << controlValue << endl;

		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			auto pShader = pMaterial->GetShader();
			pShader->Use();
			pShader->SetUniformFloat("controlValue", controlValue);
		}
	}
	else if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnMousePosition(window, xpos, ypos);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnMouseButton(window, button, action, mods);
	}

	//if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//{
	//	auto pNode = gScene->GetSceneNode("NewMesh");
	//	auto pGeometry = pNode->GetGeometry("NewMesh");

	//	double xpos, ypos;
	//	glfwGetCursorPos(window, &xpos, &ypos);

	//	auto faceIndices = pGeometry->RayIntersect(xpos, ypos, gScene->GetMainCamera()->GetProjectionMatrix(), gScene->GetMainCamera()->GetViewMatrix());
	//	//auto faceIndices = pGeometry->RayIntersect(gScene->GetMainCamera()->GetLocalPosition(), gScene->GetMainCamera()->GetCameraFront());
	//	auto faceIndex = faceIndices[0];
	//	auto vi0 = pGeometry->GetIndex(faceIndex * 3);
	//	auto vi1 = pGeometry->GetIndex(faceIndex * 3 + 1);
	//	auto vi2 = pGeometry->GetIndex(faceIndex * 3 + 2);

	//	auto v0 = pGeometry->GetVertex(vi0);
	//	auto v1 = pGeometry->GetVertex(vi1);
	//	auto v2 = pGeometry->GetVertex(vi2);
	//	auto vc = (v0 + v1 + v2) / 3.0f;
	//	auto vn = pGeometry->GetNormal(vi0);

	//	float dist = FLT_MAX;
	//	int nearestFrame = 0;
	//	for (auto& frame : frameInformations)
	//	{
	//		auto camera_info = frame->GetCameraInfo();
	//		if (camera_info->GetFrustum()->ContainsAll(v0, v1, v2))
	//		{
	//			auto& position = camera_info->GetPosition();
	//			auto distance2 = glm::distance2(vc, position);
	//			if (distance2 < dist)
	//			{
	//				dist = distance2;
	//				nearestFrame = frame->GetFrameIndex();
	//			}
	//		}
	//	}
	//	selectedFrame = nearestFrame;

	//	gScene->GetSceneNodeIMGUI()->SetText(format("{}", selectedFrame));

	//	{
	//		auto pDebugNode = gScene->GetSceneNode("Debug");
	//		auto pDebugGeometry = dynamic_cast<HeGeometryThickLines*>(pDebugNode->GetGeometry("Debug"));
	//		pDebugGeometry->AddVertex(v0);
	//		pDebugGeometry->AddVertex(v1);
	//		pDebugGeometry->AddVertex(v1);
	//		pDebugGeometry->AddVertex(v2);
	//		pDebugGeometry->AddVertex(v2);
	//		pDebugGeometry->AddVertex(v0);

	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//		pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
	//	}
	//}
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnWheel(window, xoffset, yoffset);
	}
}