#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Helium.h"

using namespace ArtificialNature;

#include <NanoVG/nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <NanoVG/nanovg_gl.h>

//#pragma comment(lib, "nanovg.lib")

const int mWidth = 1024;
const int mHeight = 1024;
//const int mWidth = 512;
//const int mHeight = 512;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

HeGraphics* gGraphics = nullptr;
HePerspectiveCamera* pCameraPerspective = nullptr;
HeOrthogonalCamera* pCameraOrtho = nullptr;
HeCamera* pCamera = nullptr;
HeCameraManipulatorFlight* pCameraManipulatorFlight = nullptr;
HeCameraManipulatorOrtho* pCameraManipulatorOrtho = nullptr;
HeCameraManipulatorBase* pCameraManipulator = nullptr;

class FrameDebugger
{
public:
    FrameDebugger(HeGraphics* gGraphics, HeScene* scene, const HeProject& project) : graphics(graphics), scene(scene), project(project) {}

    void Initialize()
    {
        const auto& frames = project.GetFrames();
        for (auto& frame : frames)
        {
            auto cameraInfo = frame->GetCameraInfo();
            auto frustum = cameraInfo->GetFrustum();
            
            auto width = cameraInfo->GetImageWidth();
            auto height = cameraInfo->GetImageHeight();
            auto fx = cameraInfo->GetFX();
            auto hiw = width * 0.5f;
            auto hih = height * 0.5f;

            auto& nr = frustum->GetNormalizedRight();
            auto& nu = frustum->GetNormalizedUp();
            auto& nf = frustum->GetNormalizedFront();
            auto& fp = frustum->GetPosition();

            auto pNode = scene->CreateSceneNode(format("frame{}", frame->GetFrameIndex()));
            frameSceneNodes.push_back(pNode);
            {
                auto pLines = gGraphics->GetGeometryThickLines(format("frame{}_Lines", frame->GetFrameIndex()));
                pLines->Initialize();
                pLines->SetThickness(1);
                pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
                pNode->AddGeometry(pLines);

                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nr * 0.01f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nu * 0.01f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nf * 0.01f);

                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));

                auto length = sqrt(fx * fx + hiw * hiw) * 0.001f;
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);

                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));

                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);

                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));

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
    }

    void ShowAll() {
        for (auto& node : frameSceneNodes)
        {
            node->SetActive(true);
        }
    }

    void HideAll() {
        for (auto& node : frameSceneNodes)
        {
            node->SetActive(true);
        }
    }

    void ShowOne(int frameIndex) {
        if (frameIndex < frameSceneNodes.size()) {
            frameSceneNodes[frameIndex]->SetActive(true);
        }
    }

    void ShowSome(const vector<int>& frameIndices) {
        for (auto& frameIndex : frameIndices)
        {
            if (frameIndex < frameSceneNodes.size()) {
                frameSceneNodes[frameIndex]->SetActive(true);
            }
        }
    }

protected:
    HeGraphics* graphics;
    HeScene* scene;
    const HeProject& project;
    vector<HeSceneNode*> frameSceneNodes;
};

class FrameSelector
{
public:
    FrameSelector(HeGraphics* gGraphics, HeScene* scene, const HeProject& project, HeGeometry* reconstructedMesh)
        : graphics(graphics), scene(scene), project(project), reconstructedMesh(reconstructedMesh)
    {
        pLinesSceneNode = scene->CreateSceneNode("Picking Debugging Lines");

        pLinesGeometry = gGraphics->GetGeometryThickLines("Gizmo");
        pLinesGeometry->Initialize();
        pLinesGeometry->SetThickness(1);
        pLinesGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pLinesSceneNode->AddGeometry(pLinesGeometry);

        //pLines->AddVertex(glm::vec3(-1024, 0, 0));
        //pLines->AddVertex(glm::vec3(1024, 0, 0));
        //pLines->AddVertex(glm::vec3(0, -1024, 0));
        //pLines->AddVertex(glm::vec3(0, 1024, 0));
        //pLines->AddVertex(glm::vec3(0, 0, -1024));
        //pLines->AddVertex(glm::vec3(0, 0, 1024));

        //pLines->AddColor(glm::vec4(1, 0, 0, 1));
        //pLines->AddColor(glm::vec4(1, 0, 0, 1));
        //pLines->AddColor(glm::vec4(0, 1, 0, 1));
        //pLines->AddColor(glm::vec4(0, 1, 0, 1));
        //pLines->AddColor(glm::vec4(0, 0, 1, 1));
        //pLines->AddColor(glm::vec4(0, 0, 1, 1));

        auto pMaterial = gGraphics->GetMaterial("Gizmo Materials");

        auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLinesGeometry->SetMaterial(pMaterial);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }

    void PickTriangle(float screenX, float screenY)
    {
        auto faceIndices = reconstructedMesh->RayIntersect(screenX, screenY, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix());
        if(faceIndices.size() != 0)
        //for (auto& fi : faceIndices)
        {
            auto fi = faceIndices[0];
            auto vi0 = reconstructedMesh->GetIndex(fi * 3);
            auto vi1 = reconstructedMesh->GetIndex(fi * 3 + 1);
            auto vi2 = reconstructedMesh->GetIndex(fi * 3 + 2);
            auto& v0 = reconstructedMesh->GetVertex(vi0);
            auto& v1 = reconstructedMesh->GetVertex(vi1);
            auto& v2 = reconstructedMesh->GetVertex(vi2);

            pLinesGeometry->AddVertex(v0);
            pLinesGeometry->AddVertex(v1);
            pLinesGeometry->AddVertex(v1);
            pLinesGeometry->AddVertex(v2);
            pLinesGeometry->AddVertex(v2);
            pLinesGeometry->AddVertex(v0);

            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
        }
    }

protected:
    HeGraphics* graphics;
    HeScene* scene;
    const HeProject& project;
    vector<HeSceneNode*> frameSceneNodes;
    HeGeometry* reconstructedMesh;

    HeSceneNode* pLinesSceneNode;
    HeThickLines* pLinesGeometry;
};

FrameSelector* pFrameSelector = nullptr;

int main(int argc, char* argv[])
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
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

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




    Helium helium("helium", mWidth, mHeight);
    gGraphics = helium.GetGraphics();

    auto pScene = helium.GetScene("Default Scene");

    {
        pCameraPerspective = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
        pCamera = pCameraPerspective;
        HeCameraManipulatorFlight manipulator(pCameraPerspective);
        pCameraPerspective->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
        pCameraManipulatorFlight = &manipulator;
        pCameraManipulator = pCameraManipulatorFlight;
        pScene->SetMainCamera(pCameraPerspective);
        pCameraManipulatorFlight->ApplyManipulation();
    }
    //{
    //    pCameraOrtho = pScene->CreateOrthogonalCamera("Main Camera", 0, 0, mWidth, mHeight);
    //    pCamera = pCameraOrtho;
    //    HeCameraManipulatorOrtho manipulator(pCameraOrtho);
    //    pCameraManipulatorOrtho = &manipulator;
    //    pCameraManipulator = pCameraManipulatorOrtho;
    //    pScene->SetMainCamera(pCameraOrtho);
    //}

    {
        auto pNode = pScene->CreateSceneNode("Gizmo Node");
        
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

    {
        auto pNode = pScene->CreateSceneNode("Mesh");
        auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\04_Fixed.stl");
        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
    }

    HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

    FrameDebugger frameDebugger(gGraphics, pScene, project);
    frameDebugger.Initialize();

    FrameSelector frameSelector(gGraphics, pScene, project, gGraphics->GetGeometry("Mesh"));
    pFrameSelector = &frameSelector;

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

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //pScene->GetSceneNode("Plane")->SetActive(true);

        pScene->Update((float)delta);
        pScene->Render();

        gGraphics->Flush();
        

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    glfwTerminate();
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

 /*       vector<string> fileNames;
        for (size_t i = 0; i < 37; i++)
        {
            stringstream ss;
            ss << "CaptureFileName_" << i << ".png";
            fileNames.push_back(ss.str());
        }

        gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
            pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
        });*/
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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if (pCameraManipulator != nullptr)
        {
            pFrameSelector->PickTriangle((float)pCameraManipulator->GetLastMousePositionX(), (float)pCameraManipulator->GetLastMousePositionY());
        }
    }
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (pCameraManipulator != nullptr)
    {
        pCameraManipulator->OnWheel(window, xoffset, yoffset);
    }
}
