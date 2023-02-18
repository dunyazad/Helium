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
//void processInput(GLFWwindow* window);

HeGraphics* gGraphics = nullptr;
HeScene* gScene = nullptr;
//HeOrthogonalCamera* pCamera = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorBase* pCameraManipulator = nullptr;
//HeCameraManipulatorOrtho* pCameraManipulator = nullptr;

int capturedFrameCount = 0;
int selectedFrame = -1;
vector<HeFrameInfo*> frameInformations;
vector<float> controlValues(256);
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

HeVisualDebugger* vd = nullptr;

HeShader* pShaderByDistance = nullptr;
HeShader* pShaderByUV = nullptr;
HeShader* pShaderBlending = nullptr;
HeShader* pShaderCustomBlending = nullptr;

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
		pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
		//pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
		pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
		pCameraManipulator = gScene->CreateCameraManipulatoObital("Main Camera Manipulator", pCamera);
		gScene->SetMainCamera(pCamera);

		vd = gScene->GetVisualDebugger();

		for (auto& v : controlValues)
		{
			v = 1.0f;
		}

		{
			auto pNode = gScene->CreateSceneNodeImgui("imgui");
			pNode->SetText("0");
		}

		{
			auto pNode = gScene->CreateSceneNode("Mesh");
			auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\04_Fixed.stl", 1000, 1000, 1000);
			//auto pGeometry = HeResourceIO::ReadOBJFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\01_MeshFromRGBD.obj");

			//pGeometry->SetFillMode(HeGeometry::Wireframe);
			pGeometry->Initialize();
			//pNode->AddGeometry(pGeometry);

			auto pMaterial = gGraphics->GetMaterial("Mesh Material");
			pGeometry->SetMaterial(pMaterial);

			auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
			pMaterial->SetShader(pShader);
		}

		{
			//HeProject project(argv[1], argv[2]);
			project = new HeProject("default", "data", "D:\\Workspace\\Reconstruct");
			capturedFrameCount = project->GetFrames().size();
			vector<float> dataToFragmentShader;


			vector<HeImage*> colorImages;
			for (size_t i = 0; i < capturedFrameCount; i++)
			{
				controlValues[i] = 1.0f;

				auto frame = project->GetFrames()[i];
				frameInformations.push_back(frame);

				dataToFragmentShader.push_back(frame->GetCameraInfo()->GetOriginalFX());

				auto frameMatrix = frame->GetCameraInfo()->GetTransformMatrix();
				auto framePosition = frame->GetCameraInfo()->GetPosition();
				dataToFragmentShader.push_back(framePosition.x);
				dataToFragmentShader.push_back(framePosition.y);
				dataToFragmentShader.push_back(framePosition.z);

				auto frameDirection = frame->GetCameraInfo()->GetRotation()[2];
				dataToFragmentShader.push_back(frameDirection.x);
				dataToFragmentShader.push_back(frameDirection.y);
				dataToFragmentShader.push_back(frameDirection.z);

				auto frameInverseTransform = glm::value_ptr(frame->GetCameraInfo()->GetInversedTransformMatrix());
				for (int j = 0; j < 16; j++) {
					dataToFragmentShader.push_back(frameInverseTransform[j]);
				}

				auto image = frame->LoadColorImage(gGraphics);
				image->Initialize();
				colorImages.push_back(image);


				auto camera_info = frame->GetCameraInfo();
				auto width = camera_info->GetColorImageWidth();
				auto height = camera_info->GetColorImageHeight();
				auto fx = camera_info->GetOriginalFX();
				auto hiw = width * 0.5f;
				auto hih = height * 0.5f;

				auto cameraInfo = frame->GetCameraInfo();
				auto frustum = cameraInfo->GetFrustum();
				auto& nr = frustum->GetRight();
				auto& nu = frustum->GetUp();
				auto& nf = frustum->GetForward();
				auto& fp = frustum->GetPosition();

				auto pNode = gScene->CreateSceneNode(format("frame{}", frame->GetFrameIndex()));
				onoff.AddSceneNode(pNode);
				{
					auto pLines = gGraphics->GetGeometryThickLines(format("frame{}_Lines", frame->GetFrameIndex()));
					pLines->Initialize();
					pLines->SetThickness(1);
					pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
					pNode->AddGeometry(pLines);

					pLines->AddVertex(fp);
					pLines->AddVertex(fp + nr * 0.1f);
					pLines->AddVertex(fp);
					pLines->AddVertex(fp + nu * 0.1f);
					pLines->AddVertex(fp);
					pLines->AddVertex(fp + nf * 0.1f);

					pLines->AddColor(HeColor::RED);
					pLines->AddColor(HeColor::RED);
					pLines->AddColor(HeColor::GREEN);
					pLines->AddColor(HeColor::GREEN);
					pLines->AddColor(HeColor::BLUE);
					pLines->AddColor(HeColor::BLUE);

					auto length = sqrt(fx * fx + hiw * hiw) * 0.01f;
					pLines->AddVertex(fp);
					pLines->AddVertex(frustum->GetImageLeftUp());
					pLines->AddVertex(fp);
					pLines->AddVertex(frustum->GetImageRightUp());
					pLines->AddVertex(fp);
					pLines->AddVertex(frustum->GetImageLeftDown());
					pLines->AddVertex(fp);
					pLines->AddVertex(frustum->GetImageRightDown());

					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);

					pLines->AddVertex(frustum->GetImageLeftUp());
					pLines->AddVertex(frustum->GetImageRightUp());
					pLines->AddVertex(frustum->GetImageRightUp());
					pLines->AddVertex(frustum->GetImageRightDown());
					pLines->AddVertex(frustum->GetImageRightDown());
					pLines->AddVertex(frustum->GetImageLeftDown());
					pLines->AddVertex(frustum->GetImageLeftDown());
					pLines->AddVertex(frustum->GetImageLeftUp());

					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);
					pLines->AddColor(HeColor::YELLOW);

					auto pMaterial = gGraphics->GetMaterial("Gizmo Materials");

					auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
					pMaterial->SetShader(pShader);

					pLines->SetMaterial(pMaterial);


					glEnable(GL_LINE_SMOOTH);
					glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

					glEnable(GL_BLEND);
					glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				}
			}

			//for (size_t i = 0; i < dataToFragmentShader.size(); i++)
			//{
			//    printf("%4d : %f\n", i, dataToFragmentShader[i]);
			//}

			HeTextureFloatData* textureFloatData = gGraphics->GetTextureFloatData("float data", dataToFragmentShader);
			textureFloatData->Initialize();

			HeTextureArray* colorTextures = gGraphics->GetTextureArray("color textures", colorImages);
			colorTextures->Initialize();

			auto pNode = gScene->CreateSceneNode("NewMesh");

			auto pGeometry = gGraphics->GetGeometryTriangleSoup("NewMesh");
			pGeometry->Initialize();
			pNode->AddGeometry(pGeometry);

			auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
			pGeometry->SetMaterial(pMaterial);
			pMaterial->SetTextureArray(colorTextures);
			pMaterial->SetTextureFloatData(textureFloatData);

			{
				auto pShader = gGraphics->GetShader("by_distance", "../../res/shader/reprojection/reprojection.vs", "../../res/shader/reprojection/by_distance.fs");
				pMaterial->SetShader(pShader);
				pShaderByDistance = pShader;

				pShader->Use();
				pShader->SetUniformInt("screenWidth", windowWidth);
				pShader->SetUniformInt("screenHeight", windowHeight);

				pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
				pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

				pShader->SetUniformFloat("controlValue", controlValue);

				pShader->SetUniformInt("frameCount", capturedFrameCount);

				pShader->SetUniformFloatArray("controlValues", controlValues);
			}
			{
				auto pShader = gGraphics->GetShader("by_uv", "../../res/shader/reprojection/reprojection.vs", "../../res/shader/reprojection/by_uv.fs");
				pShaderByUV = pShader;

				pShader->Use();
				pShader->SetUniformInt("screenWidth", windowWidth);
				pShader->SetUniformInt("screenHeight", windowHeight);

				pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
				pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

				pShader->SetUniformFloat("controlValue", controlValue);

				pShader->SetUniformInt("frameCount", capturedFrameCount);

				pShader->SetUniformFloatArray("controlValues", controlValues);
			}
			{
				auto pShader = gGraphics->GetShader("blending", "../../res/shader/reprojection/reprojection.vs", "../../res/shader/reprojection/blending.fs");
				pShaderBlending = pShader;

				pShader->Use();
				pShader->SetUniformInt("screenWidth", windowWidth);
				pShader->SetUniformInt("screenHeight", windowHeight);

				pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
				pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

				pShader->SetUniformFloat("controlValue", controlValue);

				pShader->SetUniformInt("frameCount", capturedFrameCount);

				pShader->SetUniformFloatArray("controlValues", controlValues);
			}
			{
				auto pShader = gGraphics->GetShader("custom_blending", "../../res/shader/reprojection/reprojection.vs", "../../res/shader/reprojection/custom_blending.fs");
				pShaderCustomBlending = pShader;

				pShader->Use();
				pShader->SetUniformInt("screenWidth", windowWidth);
				pShader->SetUniformInt("screenHeight", windowHeight);

				pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
				pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

				pShader->SetUniformFloat("controlValue", controlValue);

				pShader->SetUniformInt("frameCount", capturedFrameCount);

				pShader->SetUniformFloatArray("controlValues", controlValues);
			}

			auto mesh = gGraphics->GetGeometryTriangleSoup("Mesh");
			auto nof = mesh->GetFaceCount();
			for (size_t i = 0; i < nof; i++)
			{
				auto vi0 = mesh->GetIndex(i * 3);
				auto vi1 = mesh->GetIndex(i * 3 + 1);
				auto vi2 = mesh->GetIndex(i * 3 + 2);

				auto& v0 = mesh->GetVertex(vi0);
				auto& v1 = mesh->GetVertex(vi1);
				auto& v2 = mesh->GetVertex(vi2);

				auto d10 = glm::normalize(v0 - v1);
				auto d12 = glm::normalize(v2 - v1);
				auto fn = glm::normalize(glm::cross(d10, d12));

				pGeometry->AddTriangle(v0, v1, v2, fn, fn, fn);
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

		if (cnt == 20)
		{
			auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
			//auto pMaterial = dynamic_cast<HeMaterialTextureArray*>(gGraphics->GetMaterial("texture array plane"));
			pMaterial->SetIncremental(incremental);
			onoff.On(incremental);

			incremental++;
			if (incremental > capturedFrameCount) {
				incremental = 0;
			}

			cnt = 0;
		}
		cnt++;

		gScene->Update((float)delta);
		gScene->Render();

		gGraphics->Flush();

		/*       {
				   auto pNode = gScene->GetSceneNodeIMGUI();
				   pNode->SetText(format("{}", frameCount));
			   }*/

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

	auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
	if (pMaterial)
	{
		auto pShader = pMaterial->GetShader();
		pShader->Use();
		pShader->SetUniformInt("screenWidth", windowWidth);
		pShader->SetUniformInt("screenHeight", windowHeight);
	}
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
		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			pMaterial->SetShader(pShaderByDistance);
			pShaderByDistance->Use();
			pShaderByDistance->SetUniformInt("screenWidth", windowWidth);
			pShaderByDistance->SetUniformInt("screenHeight", windowHeight);
			cout << "by distance" << endl;
		}
	}
	else if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			pMaterial->SetShader(pShaderByUV);
			pShaderByUV->Use();
			pShaderByUV->SetUniformInt("screenWidth", windowWidth);
			pShaderByUV->SetUniformInt("screenHeight", windowHeight);
			cout << "by uv" << endl;
		}
	}
	else if (key == GLFW_KEY_3 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			pMaterial->SetShader(pShaderBlending);
			pShaderBlending->Use();
			pShaderBlending->SetUniformInt("screenWidth", windowWidth);
			pShaderBlending->SetUniformInt("screenHeight", windowHeight);
			cout << "blending" << endl;
		}
	}
	else if (key == GLFW_KEY_4 && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
		if (pMaterial)
		{
			pMaterial->SetShader(pShaderCustomBlending);
			pShaderCustomBlending->Use();
			pShaderCustomBlending->SetUniformInt("screenWidth", windowWidth);
			pShaderCustomBlending->SetUniformInt("screenHeight", windowHeight);
			cout << "custom blending" << endl;
		}
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
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			controlValues[selectedFrame] += 0.01;
		}
		else
		{
			controlValues[selectedFrame] += 0.001;
		}

		pShaderByDistance->Use();
		pShaderByDistance->SetUniformFloatArray("controlValues", controlValues);

		pShaderByUV->Use();
		pShaderByUV->SetUniformFloatArray("controlValues", controlValues);

		pShaderBlending->Use();
		pShaderBlending->SetUniformFloatArray("controlValues", controlValues);

		pShaderCustomBlending->Use();
		pShaderCustomBlending->SetUniformFloatArray("controlValues", controlValues);

		cout << "controlValues[" << selectedFrame << "] : " << controlValues[selectedFrame] << endl;
	}
	else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			controlValues[selectedFrame] -= 0.01;
		}
		else
		{
			controlValues[selectedFrame] -= 0.001;
		}

		pShaderByDistance->Use();
		pShaderByDistance->SetUniformFloatArray("controlValues", controlValues);

		pShaderByUV->Use();
		pShaderByUV->SetUniformFloatArray("controlValues", controlValues);

		pShaderBlending->Use();
		pShaderBlending->SetUniformFloatArray("controlValues", controlValues);

		pShaderCustomBlending->Use();
		pShaderCustomBlending->SetUniformFloatArray("controlValues", controlValues);

		cout << "controlValues[" << selectedFrame << "] : " << controlValues[selectedFrame] << endl;
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

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		auto pNode = gScene->GetSceneNode("NewMesh");
		auto pGeometry = pNode->GetGeometry("NewMesh");

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		auto faceIndices = pGeometry->RayIntersect(xpos, ypos, gScene->GetMainCamera()->GetProjectionMatrix(), gScene->GetMainCamera()->GetViewMatrix());
		//auto faceIndices = pGeometry->RayIntersect(gScene->GetMainCamera()->GetLocalPosition(), gScene->GetMainCamera()->GetCameraFront());
		auto faceIndex = faceIndices[0];
		auto vi0 = pGeometry->GetIndex(faceIndex * 3);
		auto vi1 = pGeometry->GetIndex(faceIndex * 3 + 1);
		auto vi2 = pGeometry->GetIndex(faceIndex * 3 + 2);

		auto v0 = pGeometry->GetVertex(vi0);
		auto v1 = pGeometry->GetVertex(vi1);
		auto v2 = pGeometry->GetVertex(vi2);
		auto vc = (v0 + v1 + v2) / 3.0f;
		auto vn = pGeometry->GetNormal(vi0);

		float dist = FLT_MAX;
		int nearestFrame = 0;
		for (auto& frame : frameInformations)
		{
			auto camera_info = frame->GetCameraInfo();
			if (camera_info->GetFrustum()->ContainsAll(v0, v1, v2))
			{
				auto& position = camera_info->GetPosition();
				auto distance2 = glm::distance2(vc, position);
				if (distance2 < dist)
				{
					dist = distance2;
					nearestFrame = frame->GetFrameIndex();
				}
			}
		}
		selectedFrame = nearestFrame;

		gScene->GetSceneNodeIMGUI()->SetText(format("{}", selectedFrame));

		vd->AddTriangle(v0, v1, v2, HeColor::RED, HeColor::RED, HeColor::RED);
	}
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnWheel(window, xoffset, yoffset);
	}
}
