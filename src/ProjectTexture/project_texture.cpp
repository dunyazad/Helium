#include <Helium/Helium.h>
using namespace ArtificialNature;

const int windowWidth = 1024;
const int windowHeight = 1024;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);

HeGraphics* gGraphics = nullptr;
HeScene* gScene = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorBase* pCameraManipulator = nullptr;

HeVisualDebugger* vd = nullptr;

int main(int argc, char** argv)
{
	cout << "arguments" << endl;
	for (size_t i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}
	cout << endl;

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
	HeSettings["Current Working Directory"] = "./";

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
		vd->AddAxisLines();

	{
		auto pNode = gScene->CreateSceneNode("Mesh");
		auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", argv[4], 1000, 1000, 1000);

		if (pGeometry == nullptr) {
			cout << "pGeometry == nullptr" << endl;
		}

		//pGeometry->SetFillMode(HeGeometry::Wireframe);
		pGeometry->Initialize();
		pNode->AddGeometry(pGeometry);

		auto pMaterial = gGraphics->GetMaterial("Mesh Material");
		pGeometry->SetMaterial(pMaterial);

		auto pShader = gGraphics->GetShader("vertex", HeURL::GetShaderFileURL("vertex.vs"), HeURL::GetShaderFileURL("vertex.fs"));
		pMaterial->SetShader(pShader);

		//HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:/Resources/Scan/projects/default/data/reconstructed/TestOBJ.obj");
	}

	{
		HeProject project(argv[2], argv[3]);
		auto frames = project.GetFrames();

		cout << "number of frames " << frames.size() << endl;

		auto DrawFrameOutline = [&](HeProject* project, int frameIndex, const HeColor& color) {
			auto frame = frames[frameIndex];
			auto cameraInfo = frame->GetCameraInfo();
			auto frustum = cameraInfo->GetFrustum();

			//vd->AddLine(cameraInfo->GetPosition(), cameraInfo->UVToWorld(glm::vec2(0.25, 0.75)), HeColor::CYAN, HeColor::CYAN);

			auto pc = frustum->GetPosition();
			auto ic = frustum->GetImageCenter();
			auto ilu = frustum->GetImageLeftUp();
			auto ild = frustum->GetImageLeftDown();
			auto iru = frustum->GetImageRightUp();
			auto ird = frustum->GetImageRightDown();

			vd->AddLine(pc, ic, color, color);
			vd->AddLine(pc, ilu, color, color);
			vd->AddLine(pc, ild, color, color);
			vd->AddLine(pc, iru, color, color);
			vd->AddLine(pc, ird, color, color);

			cout << "ilu : " << ilu << endl;
			cout << "ild : " << ild << endl;
			cout << "iru : " << iru << endl;
			cout << "ird : " << ird << endl;

			//if (frameIndex == 4)
			//{
			//    auto tw = HeColor(1.0f, 1.0f, 1.0f, 1.0f);
			//    vd->AddTriangle(pc, ild, ilu, tw);
			//    vd->AddTriangle(pc, ilu, iru, tw);
			//    vd->AddTriangle(pc, iru, ird, tw);
			//    vd->AddTriangle(pc, ird, ild, tw);
			//}

			vd->AddLine(ilu, iru, color, color);
			vd->AddLine(iru, ird, color, color);
			vd->AddLine(ird, ild, color, color);
			vd->AddLine(ild, ilu, color, color);
			vd->AddHalfAxisLines(cameraInfo->GetTransformMatrix(), 100, 100, 100);
		};


		vector<HeGeometryTriangleSoup*> frameGeometries;
		for (size_t i = 0; i < frames.size(); i++)
		{
			auto& frame = frames[i];

			auto id = format("frame_{}", frame->GetFrameIndex());
			auto pNode = gScene->CreateSceneNode(id);
			auto pGeometry = gGraphics->GetGeometryTriangleSoup(id);
			pGeometry->Initialize();
			frameGeometries.push_back(pGeometry);
			pNode->AddGeometry(pGeometry);

			auto pMaterial = gGraphics->GetMaterialSingleTexture(id);
			pGeometry->SetMaterial(pMaterial);

			auto pShader = gGraphics->GetShader("texture", HeURL::GetShaderFileURL("texture.vs"), HeURL::GetShaderFileURL("texture.fs"));
			pMaterial->SetShader(pShader);

			auto image = frame->LoadColorImage(gGraphics);
			auto pTexture = gGraphics->GetTexture(id, image);
			pTexture->Initialize();
			pMaterial->SetTexture(pTexture);

			DrawFrameOutline(&project, i, HeColor::RED);
		}

		auto mesh = gGraphics->GetGeometry("Mesh");
		auto nof = mesh->GetFaceCount();
		for (size_t fi = 0; fi < nof; fi++)
		{
			auto vi0 = mesh->GetIndex(fi * 3);
			auto vi1 = mesh->GetIndex(fi * 3 + 1);
			auto vi2 = mesh->GetIndex(fi * 3 + 2);

			const auto& v0 = mesh->GetVertex(vi0);
			const auto& v1 = mesh->GetVertex(vi1);
			const auto& v2 = mesh->GetVertex(vi2);
			auto fc = (v0 + v1 + v2) / 3.0f;
			auto fn = mesh->GetNormal(vi0);

			HeFrameInfo* nearestFrame = nullptr;
			float nearestDistance2 = FLT_MAX;

			HeFrameInfo* oppositeFacingFrame = nullptr;
			float dotBetweenCameraAndFaceNormal = FLT_MAX;

			HeFrameInfo* nearUVCenterFrame = nullptr;
			float uvCenterDistance2 = FLT_MAX;
			for (auto& frame : frames)
			{
				auto cameraInfo = frame->GetCameraInfo();
				auto cameraFront = glm::vec3(cameraInfo->GetViewMatrix()[3]);
				auto frustum = cameraInfo->GetFrustum();

				if (frustum->ContainsAll(v0, v1, v2))
				{
					auto ffd = glm::distance2(fc, frame->GetCameraInfo()->GetPosition());
					if (ffd < nearestDistance2) {
						nearestDistance2 = ffd;
						nearestFrame = frame;
					}

					auto dot = glm::dot(cameraFront, fn);
					if (dotBetweenCameraAndFaceNormal > dot) {
						dotBetweenCameraAndFaceNormal = dot;
						oppositeFacingFrame = frame;
					}

					auto uvc = cameraInfo->WorldToUV(fc);

					auto dc = glm::distance2(uvc, glm::vec2(0.5f, 0.5f));

					if (uvCenterDistance2 > dc) {
						uvCenterDistance2 = dc;
						nearUVCenterFrame = frame;
					}
				}
			}

			//if (nearestFrame != nullptr)
			//{
			//    auto cameraInfo = nearestFrame->GetCameraInfo();
			//    auto uv0 = cameraInfo->WorldToUV(v0);
			//    auto uv1 = cameraInfo->WorldToUV(v1);
			//    auto uv2 = cameraInfo->WorldToUV(v2);

			//    auto pGeometry = frameGeometries[nearestFrame->GetFrameIndex()];
			//    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
			//}
			//if (oppositeFacingFrame != nullptr)
			//{
			//    auto cameraInfo = oppositeFacingFrame->GetCameraInfo();
			//    auto uv0 = cameraInfo->WorldToUV(v0);
			//    auto uv1 = cameraInfo->WorldToUV(v1);
			//    auto uv2 = cameraInfo->WorldToUV(v2);

			//    auto pGeometry = frameGeometries[oppositeFacingFrame->GetFrameIndex()];
			//    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
			//}
			if (nearUVCenterFrame != nullptr)
			{
				auto cameraInfo = nearUVCenterFrame->GetCameraInfo();
				auto uv0 = cameraInfo->WorldToUV(v0);
				auto uv1 = cameraInfo->WorldToUV(v1);
				auto uv2 = cameraInfo->WorldToUV(v2);

				auto pGeometry = frameGeometries[nearUVCenterFrame->GetFrameIndex()];
				pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
			}
		}

		HeResourceIO::WriteOBJFile(gGraphics, frameGeometries, argv[5]);
	}
		});


		int cnt = 0;

		auto lastTime = HeTime::Now();
		double accTime = 0.0;
		int frameCount = 0;
		int fps = 0;
		int incremental = 0;

		//helium.SetFinished(true);
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

		glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
		//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	system("break");

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	pCamera->SetAspectRatio((float)width / (float)height);

	//gGraphics->GetFrameBuffer("FrameBuffer")->Resize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnKey(window, key, scancode, action, mods);
	}

	if (key == '1' && action == GLFW_RELEASE)
	{
		auto pGeometry = gGraphics->GetGeometry("Mesh");
		if (pGeometry != nullptr)
		{
			if (pGeometry->GetFillMode() == HeGeometry::FillMode::Fill)
			{
				pGeometry->SetFillMode(HeGeometry::FillMode::Wireframe);
			}
			else if (pGeometry->GetFillMode() == HeGeometry::FillMode::Wireframe)
			{
				pGeometry->SetFillMode(HeGeometry::FillMode::Fill);
			}
		}
	}
	else if (key == GLFW_KEY_C && action == GLFW_RELEASE)
	{
		//auto image = gGraphics->GetCanvasImage("Capture", mWidth, mHeight);
		//image->CaptureFrame("capture.png");

		vector<string> fileNames;
		for (size_t i = 0; i < 256; i++)
		{
			stringstream ss;
			ss << "texture_" << i << ".png";
			fileNames.push_back(ss.str());
		}

		gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
			pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
			});
	}
	else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		//onoff.Previous();
	}
	else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		//onoff.Next();
	}
	else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
	}
	else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
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
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (pCameraManipulator != nullptr)
	{
		pCameraManipulator->OnWheel(window, xoffset, yoffset);
	}
}
