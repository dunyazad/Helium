#define USING_FILES_SYSTEM
#include <Helium/Helium.h>
#include <omp.h>
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


typedef unsigned char uchar;


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


    gGraphics = helium.GetGraphics();
    gScene = helium.GetScene("Default Scene");

    helium.OnPrepare([&]() {
        pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = gScene->CreateCameraManipulatoObital("Main Camera Manipulator", pCamera);
    gScene->SetMainCamera(pCamera);

    vd = gScene->GetVisualDebugger();
    //onoff = vd->GetOnOff();

    auto project = new HeProject("default", "2023-04-25_14-27-49", "C:/Users/Mickey/Desktop/bitbucket/spacecapture/Server");
    auto frames = project->GetFrames();

    vector<float> exposureOffsets;
    int minExposureFrame = -1;
    int maxExposureFrame = -1;
    float minExposureOffset = FLT_MAX;
    float maxExposureOffset = -FLT_MAX;
    for (size_t i = 0; i < frames.size(); i++)
    {
        auto frame = frames[i];
        auto cameraInfoPath = HeURL(project->GetCameraInfoPath());
        auto camjPath = cameraInfoPath + format("/camera_info_{}.camj", i);

        std::ifstream f(camjPath.path);
        json camj = json::parse(f);
        if (camj.contains("exposureOffset"))
        {
            auto exposureOffset = camj["exposureOffset"].get<float>();
            exposureOffsets.push_back(exposureOffset);

            if (exposureOffset < minExposureOffset) {
                minExposureOffset = exposureOffset;
                minExposureFrame = i;
            }

            if (exposureOffset > maxExposureOffset) {
                maxExposureOffset = exposureOffset;
                maxExposureFrame = i;
            }
        }
    }

    float meanExposureOffset = (minExposureOffset + maxExposureOffset) * 0.5f;
    cout << "min Exposure Frame: " << minExposureFrame << endl;
    cout << "max Exposure Frame: " << maxExposureFrame << endl;

    for (size_t i = 0; i < frames.size(); i++)
    {
        float ev = (minExposureOffset / exposureOffsets[i]) - 1.0f;
        //float ev = (maxExposureOffset / exposureOffsets[i]) - 1.0f;
        //float ev = (meanExposureOffset / exposureOffsets[i]) - 1.0f;

        auto colorPath = HeURL(project->GetColorPath());
        auto colorImagePath = colorPath + format("/color_{}.jpg", i);

        auto image = gGraphics->GetImage(format("color_{}", i), colorImagePath.path);
        image->Initialize();

        auto width = image->GetWidth();
        auto height = image->GetHeight();
        auto noc = image->GetChannels();
        auto imageData = image->Data();
        for (int h = 0; h < height; h++)
        {
            for (int w = 0; w < width; w++)
            {
                auto pixel = &(imageData[h * width * noc + w * noc]);
                //auto r = uchar(((float)h / (float)height) * 255.0f);
                //auto g = uchar(((float)w / (float)width) * 255.0f);
                //auto b = uchar(((float)h / (float)height * (float)w / (float)width) * 255.0f);

                auto r = pixel[0] * powf(2.0, ev);
                auto g = pixel[1] * powf(2.0, ev);
                auto b = pixel[2] * powf(2.0, ev);

                if (r < 0.0f) r = 0.0f; if (r > 255) r = 255;
                if (g < 0.0f) g = 0.0f; if (g > 255) g = 255;
                if (b < 0.0f) b = 0.0f; if (b > 255) b = 255;

                *(pixel + 0) = r;
                *(pixel + 1) = g;
                *(pixel + 2) = b;
            }
        }

        auto resultPath = HeURL(project->GetResultPath());
        auto imageOutPath = resultPath + format("frame{}.png", i);
        image->Write(imageOutPath.path);
        cout << imageOutPath.path << " saved." << endl;
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

    glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
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

    helium.SetFinished(true);

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
        //onoff->Previous();
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        //onoff->Next();
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
