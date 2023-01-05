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
HeCameraManipulatorFlight* pCameraManipulator = nullptr;
//HeCameraManipulatorOrtho* pCameraManipulator = nullptr;

int capturedFrameCount = 0;
float controlValue = 0.0f;
HeSceneNode* selectedSceneNode = nullptr;

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
    gGraphics = helium.GetGraphics();

    helium.OnPrepare([&]() {
        cout << "Helium::OnPrepare" << endl;
    });

    //helium.Run();

    helium.InitializeImgui(mWindow);

    gScene = helium.GetScene("Default Scene");

    pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    gScene->SetMainCamera(pCamera);

    {
        auto pNode = gScene->CreateSceneNode("Gizmo Node");

#pragma region [Lines]
        auto pLines = gGraphics->GetGeometryThickLines("Gizmo");
        pLines->Initialize();
        pLines->SetThickness(1);
        pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pNode->AddGeometry(pLines);

        pLines->AddVertex(glm::vec3(-1024, 0, 0));
        pLines->AddVertex(glm::vec3(1024, 0, 0));
        pLines->AddVertex(glm::vec3(0, -1024, 0));
        pLines->AddVertex(glm::vec3(0, 1024, 0));
        pLines->AddVertex(glm::vec3(0, 0, -1024));
        pLines->AddVertex(glm::vec3(0, 0, 1024));

        pLines->AddColor(glm::vec4(1, 0, 0, 1));
        pLines->AddColor(glm::vec4(1, 0, 0, 1));
        pLines->AddColor(glm::vec4(0, 1, 0, 1));
        pLines->AddColor(glm::vec4(0, 1, 0, 1));
        pLines->AddColor(glm::vec4(0, 0, 1, 1));
        pLines->AddColor(glm::vec4(0, 0, 1, 1));

        auto pMaterial = gGraphics->GetMaterial("Gizmo Materials");

        auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLines->SetMaterial(pMaterial);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
#pragma endregion
    }

    HeProject project("default", "data", "D:\\Workspace\\Reconstruct");
    capturedFrameCount = project.GetFrames().size();
    vector<float> dataToFragmentShader;

    vector<HeImage*> colorImages;
    for (size_t i = 0; i < capturedFrameCount; i++)
    {
        auto frame = project.GetFrames()[i];
         
        auto image = frame->LoadColorImage(gGraphics);
        image->Initialize();

        colorImages.push_back(image);

        //auto pNode = gScene->CreateSceneNode(format("plane{}", i));

        //auto pGeometry = gGraphics->GetGeometryPlane(format("plane{}", i), .25, .25, 10, 10, HePlaneType::XY);
        //pGeometry->Initialize();
        //pNode->AddGeometry(pGeometry);

        //auto pMaterial = gGraphics->GetMaterialSingleTexture(format("plane{}", i));
        //pGeometry->SetMaterial(pMaterial);

        //auto pShader = gGraphics->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
        //pMaterial->SetShader(pShader);

        //auto pTexture = gGraphics->GetTexture(format("plane{}", i), image);
        //pTexture->Initialize();
        //pMaterial->SetTexture(pTexture);

        //const auto& r = frame->GetCameraInfo()->GetRotation();
        //auto r0 = glm::row(r, 0);
        //auto r1 = -glm::row(r, 1);
        //auto r2 = -glm::row(r, 2);

        //glm::mat3 rr;
        //rr[0] = r0;
        //rr[1] = r1;
        //rr[2] = r2;

        //auto angle = glm::angle(glm::vec3(0, 0, 1), rr[2]);
        //pNode->SetLocalRotation(glm::angleAxis(angle, glm::vec3(-1, 0, 0)));
        //pNode->SetLocalPosition(frame->GetCameraInfo()->GetPosition());
    }

    {
        auto pNode = gScene->CreateSceneNode("plane0");

        auto pGeometry = gGraphics->GetGeometryPlane("plane0", 1, 1, 10, 10, HePlaneType::XY);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterialSingleTexture("plane0");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("colorAdjustment0", "../../res/shader/colorAdjustment.vs", "../../res/shader/colorAdjustment.fs");
        pMaterial->SetShader(pShader);

        auto pTexture = gGraphics->GetTexture("plane0", colorImages[1]);
        pTexture->Initialize();
        pMaterial->SetTexture(pTexture);

        //auto& frame = project.GetFrames()[1];
        //pNode->SetLocalRotation(frame->GetCameraInfo()->GetRotation());
        //pNode->SetLocalPosition(frame->GetCameraInfo()->GetPosition());
    }

    {
        auto pNode = gScene->CreateSceneNode("plane1");

        auto pGeometry = gGraphics->GetGeometryPlane("plane1", 1, 1, 10, 10, HePlaneType::XY);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterialSingleTexture("plane1");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("colorAdjustment1", "../../res/shader/colorAdjustment.vs", "../../res/shader/colorAdjustment.fs");
        pMaterial->SetShader(pShader);

        auto pTexture = gGraphics->GetTexture("plane1", colorImages[2]);
        pTexture->Initialize();
        pMaterial->SetTexture(pTexture);

        //auto& frame = project.GetFrames()[2];
        //pNode->SetLocalRotation(frame->GetCameraInfo()->GetRotation());
        //pNode->SetLocalPosition(frame->GetCameraInfo()->GetPosition());
    }

    auto lastTime = HeTime::Now();
    double accTime = 0.0;
    int frameCount = 0;
    int fps = 0;
    while (glfwWindowShouldClose(mWindow) == false) {
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

        gScene->Update((float)delta);
        gScene->Render();

        gScene->UpdateImgui((float)delta);
        gScene->RenderImgui();

        gGraphics->Flush();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    helium.TerminateImgui();

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    pCamera->SetAspectRatio((float)width / (float)height);
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
    else if (key == GLFW_KEY_1 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        selectedSceneNode = gScene->GetSceneNode("plane0");
    }
    else if (key == GLFW_KEY_2 && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        selectedSceneNode = gScene->GetSceneNode("plane1");
    }
    else if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (selectedSceneNode != nullptr)
        {
            selectedSceneNode->SetLocalPosition(selectedSceneNode->GetLocalPosition() + glm::vec3(-0.01f, 0.0f, 0.0f));
        }
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (selectedSceneNode != nullptr)
        {
            selectedSceneNode->SetLocalPosition(selectedSceneNode->GetLocalPosition() + glm::vec3(0.01f, 0.0f, 0.0f));
        }
    }
    else if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (selectedSceneNode != nullptr)
        {
            selectedSceneNode->SetLocalPosition(selectedSceneNode->GetLocalPosition() + glm::vec3(0.0f, 0.01f, 0.0f));
        }
    }
    else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (selectedSceneNode != nullptr)
        {
            selectedSceneNode->SetLocalPosition(selectedSceneNode->GetLocalPosition() + glm::vec3(0.0f, -0.01f, 0.0f));
        }
    }
    else if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        {
            controlValue += 0.001f;
        }
        else
        {
            controlValue += 0.01f;
        }

        if (selectedSceneNode != nullptr)
        {
            auto pGeometry = selectedSceneNode->GetGeometry(selectedSceneNode->GetName());
            auto pMaterial = pGeometry->GetMaterial();
            auto pShader = pMaterial->GetShader();
            pShader->Use();
            pShader->SetUniformFloat("controlValue", controlValue);

            cout << "control value : " << controlValue << endl;
        }
    }
    else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        {
            controlValue -= 0.001f;
        }
        else
        {
            controlValue -= 0.01f;
        }

        if (selectedSceneNode != nullptr)
        {
            auto pGeometry = selectedSceneNode->GetGeometry(selectedSceneNode->GetName());
            auto pMaterial = pGeometry->GetMaterial();
            auto pShader = pMaterial->GetShader();
            pShader->Use();
            pShader->SetUniformFloat("controlValue", controlValue);

            cout << "control value : " << controlValue << endl;
        }
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

