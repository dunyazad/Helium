#define USING_FILES_SYSTEM
#include <Helium/Helium.h>
using namespace ArtificialNature;

const int windowWidth = 1024;
const int windowHeight = 1024;

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
        pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
        //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
        pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
        pCameraManipulator = gScene->CreateCameraManipulatoObital("Main Camera Manipulator", pCamera);
        gScene->SetMainCamera(pCamera);

        vd = gScene->GetVisualDebugger();
        vd->AddAxisLines();

 /*       {
            auto pNode = gScene->CreateSceneNode("Mesh");
            auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:/Resources/Scan/projects/default/data/reconstructed/04_Fixed.stl");

            pGeometry->SetFillMode(HeGeometry::Wireframe);
            pGeometry->Initialize();
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterial("Mesh Material");
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("vertex", HeURL::GetShaderFileURL("vertex.vs", HeURL::GetShaderFileURL("vertex.fs");
            pMaterial->SetShader(pShader);
        }*/

        auto project = new HeProject("default", "Body", "D:/Resources/Scan");
        auto frames = project->GetFrames();

  /*      auto DrawFramePixels = [&](HeProject* project, int frameIndex) {
            auto frame = frames[frameIndex];
            auto image = frame->LoadColorImage(gGraphics);

            for (size_t h = 0; h < image->GetHeight(); h++)
            {
                for (size_t w = 0; w < image->GetWidth(); w++)
                {

                }
            }
        };*/

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

        auto DrawFrameImage = [&](HeProject* project, int frameIndex) {
            auto frame = frames[frameIndex];
            auto cameraInfo = frame->GetCameraInfo();
            auto frustum = cameraInfo->GetFrustum();

            auto ilu = frustum->GetImageLeftUp();
            auto ild = frustum->GetImageLeftDown();
            auto iru = frustum->GetImageRightUp();
            auto ird = frustum->GetImageRightDown();

            auto image = frame->LoadColorImage(gGraphics);
            auto texture = gGraphics->GetTexture(format("frame_{}", frameIndex), image);
            texture->Initialize();

            vd->AddPlane(ilu, ild, iru, ird, texture);
        };

        auto DrawOverlap = [&](HeProject* project, int frameA, int frameB) {
            auto pGeometry = gGraphics->GetGeometry("Mesh");
            auto nof = pGeometry->GetFaceCount();
            for (size_t i = 0; i < nof; i++)
            {
                auto i0 = pGeometry->GetIndex(i * 3);
                auto i1 = pGeometry->GetIndex(i * 3 + 1);
                auto i2 = pGeometry->GetIndex(i * 3 + 2);

                auto& v0 = pGeometry->GetVertex(i0);
                auto& v1 = pGeometry->GetVertex(i1);
                auto& v2 = pGeometry->GetVertex(i2);

                if (frames[frameA]->ContainsAny(v0, v1, v2) && frames[frameB]->ContainsAny(v0, v1, v2))
                {
                    vd->AddTriangle(v0, v1, v2, HeColor::MAGENTA);
                }
                else if (frames[frameA]->ContainsAny(v0, v1, v2))
                {
                    vd->AddTriangle(v0, v1, v2, HeColor::RED);
                }
                else if (frames[frameB]->ContainsAny(v0, v1, v2))
                {
                    vd->AddTriangle(v0, v1, v2, HeColor::BLUE);
                }
            }

            DrawFrameOutline(project, frameA, HeColor::RED);
            DrawFrameOutline(project, frameB, HeColor::BLUE);
        };

        //DrawFramePixels(project, 3);
        DrawFrameImage(project, 1);
        DrawFrameImage(project, 2);
        DrawFrameOutline(project, 1, HeColor::RED);
        DrawFrameOutline(project, 2, HeColor::BLUE);
        //DrawOverlap(project, 3, 4);

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

    return EXIT_SUCCESS;
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
