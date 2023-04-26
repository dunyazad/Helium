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
HeCameraManipulatorObital* pCameraManipulator = nullptr;

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
        pCameraManipulator = gScene->CreateCameraManipulatoObital("Main Camera Manipulator", pCamera);
        pCameraManipulator->SetDistance(5);
        pCameraManipulator->SetAngleH(45.0f);
        pCameraManipulator->SetAngleV(-45.0f);
        pCameraManipulator->ApplyCamera();

        gScene->SetMainCamera(pCamera);

        vd = gScene->GetVisualDebugger();
        vd->AddAxisLines();

        {
            auto pNode = gScene->CreateSceneNode("Mesh");
            auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", HeURL("D:/Resources/Scan/projects/default/data/reconstructed/04_Fixed.stl"), 1000, 1000, 1000);

            //pGeometry->SetFillMode(HeGeometry::Wireframe);
            pGeometry->Initialize();
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterial("Mesh Material");
            pGeometry->SetMaterial(pMaterial);

            //auto pShader = gGraphics->GetShader("vertex", HeURL::GetShaderFileURL("vertex.vs", HeURL::GetShaderFileURL("vertex.fs");
            auto pShader = gGraphics->GetShader("normal_visualization", HeURL::GetShaderFileURL("normal_visualization.vs"), HeURL::GetShaderFileURL("normal_visualization.fs"), HeURL::GetShaderFileURL("normal_visualization.gs"));
            pMaterial->SetShader(pShader);
        }

        //{
        //    auto pNode = gScene->CreateSceneNode("CubeGeometryShader");
        //    auto pGeometry = gGraphics->GetGeometry("CubeGeometryShader");
        //    pGeometry->AddVertex(glm::vec3(0, 0, 0));
        //    //pGeometry->AddVertex(glm::vec3(0, 1, 0));
        //    //pGeometry->AddVertex(glm::vec3(0, 2, 0));
        //    pGeometry->AddColor(HeColor::BLUE);
        //    pGeometry->SetDrawingMode(HeGeometry::Points);
        //    pGeometry->Initialize();
        //    pNode->AddGeometry(pGeometry);

        //    auto pMaterial = gGraphics->GetMaterial("CubeGeometryShader");
        //    pGeometry->SetMaterial(pMaterial);

        //    auto pShader = gGraphics->GetShader("CubeGeometryShader", HeURL::GetShaderFileURL("CubeGeometryShader.vs", HeURL::GetShaderFileURL("CubeGeometryShader.gs", HeURL::GetShaderFileURL("CubeGeometryShader.fs");
        //    pMaterial->SetShader(pShader);
        //}

        });

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

            //auto angleH = pCameraManipulator->GetAngleH();
            //auto angleV = pCameraManipulator->GetAngleV();
            //auto distance = pCameraManipulator->GetDistance();
            //pNode->SetText(format("H:{:.2f}, V:{:.2f}\nD:{:.2f} {}", angleH, angleV, distance, frameCount));

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
