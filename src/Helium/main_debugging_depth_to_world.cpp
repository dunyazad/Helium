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

//const int mWidth = 1024;
//const int mHeight = 1024;
const int mWidth = 1024;
const int mHeight = 1024;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

HeGraphics* gGraphics = nullptr;
//HeOrthogonalCamera* pCamera = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorFlight* pCameraManipulator = nullptr;
//HeCameraManipulatorOrtho* pCameraManipulator = nullptr;

class OnOff {
public:
    void AddSceneNode(HeSceneNode* node, const glm::vec3& position) {
        nodes.push_back(node);
        positions.push_back(position);
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

            cout << positions[index] << endl;
        }
    }

    void Previous()
    {
        if (index > 0) {
            HideAll();

            nodes[index]->SetActive(false);
            nodes[index - 1]->SetActive(true);
            index--;

            cout << positions[index] << endl;
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
    vector<glm::vec3> positions;
    int index = 0;
};

OnOff onoff;

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

    pCamera = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
    //pCamera = pScene->CreateOrthogonalCamera("Main Camera", 0, 0, mWidth, mHeight);
    HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    pScene->SetMainCamera(pCamera);

    
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
        //pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
    }

 /*   {
        auto pNode = pScene->CreateSceneNode("Point");
        auto pGeometry = gGraphics->GetGeometryPlane("Point", 0.01, 0.01, 1, 1, HePlaneType::XY);

        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Point Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertexColor", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
        pMaterial->SetShader(pShader);
    }*/

    {
        HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

        auto frames = project.GetFrames();
        for (auto& frame : frames)
        {
            auto cameraInfo = frame->GetCameraInfo();
            auto frustum = cameraInfo->GetFrustum();
            frame->LoadColorImage(gGraphics);
            auto image = frame->GetColorImage();
            image->Initialize();
            auto texture = gGraphics->GetTexture(image->GetName(), image);
            texture->Initialize();

            auto& nr = frustum->GetNormalizedRight();
            auto& nu = frustum->GetNormalizedUp();
            auto& nf = frustum->GetNormalizedFront();
            auto& fp = frustum->GetPosition();

            auto pNode = pScene->CreateSceneNode(format("frame{}", frame->GetFrameIndex()));
            onoff.AddSceneNode(pNode, cameraInfo->GetPosition());
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

                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));

                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * 1.5f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * 1.5f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * 1.5f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * 1.5f);

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
            /*
            {
                const auto& depthInfo = frame->GetDepthInfos();
                int width = frame->GetCameraInfo()->GetImageWidth();
                int height = frame->GetCameraInfo()->GetImageHeight();

                stringstream ss;
                ss << "depth_" << frame->GetFrameIndex();
                auto pGeometry = gGraphics->GetGeometryPlane(ss.str(), 1, 1, width, height, HePlaneType::XY);
                pGeometry->Initialize();
                pGeometry->SetFillMode(HeGeometry::Wireframe);

                for (size_t h = 0; h < height; h++)
                {
                    for (size_t w = 0; w < width; w++)
                    {
                        auto index = h * width + w;
                        auto d = depthInfo[(height - h) * width + w];
                        auto v = pGeometry->GetVertex(index);
                        v.z = d;

                        auto p = glm::mat4(frame->GetCameraInfo()->GetIntrinsicMatrix()) * frame->GetCameraInfo()->GetExtrinsicMatrix();
                        pGeometry->SetVertex(index, v);
                    }
                }

                pNode->AddGeometry(pGeometry);

                auto pMaterial = gGraphics->GetMaterial("Mesh Material");

                pGeometry->SetMaterial(pMaterial);

            }
            */
            {
                auto pGeometry = gGraphics->GetGeometryTriangleSoup(format("frame{}_Triangles", frame->GetFrameIndex()));
                pGeometry->Initialize();
                pNode->AddGeometry(pGeometry);
                auto pMaterial = gGraphics->GetMaterialSingleTexture(format("frame{}_SingleTexture", frame->GetFrameIndex()));
                auto pShader = gGraphics->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
                pMaterial->SetShader(pShader);
                pMaterial->SetTexture(texture);
                pGeometry->SetMaterial(pMaterial);

                auto pMesh = gGraphics->GetGeometry("Mesh");
                auto nof = pMesh->GetIndexCount() / 3;
                for (size_t i = 0; i < nof; i++)
                {
                    auto vi0 = pMesh->GetIndex(i * 3);
                    auto vi1 = pMesh->GetIndex(i * 3 + 1);
                    auto vi2 = pMesh->GetIndex(i * 3 + 2);

                    auto& v0 = pMesh->GetVertex(vi0);
                    auto& v1 = pMesh->GetVertex(vi1);
                    auto& v2 = pMesh->GetVertex(vi2);

                    vector<glm::vec3> vertices = { v0, v1, v2 };
                    if (frustum->ContainsAll(vertices))
                    {
                        //auto uv0 = cameraInfo->WorldToUV(v0);
                        //auto uv1 = cameraInfo->WorldToUV(v1);
                        //auto uv2 = cameraInfo->WorldToUV(v2);

                        auto uv0 = 
                            (glm::mat4(frame->GetCameraInfo()->GetIntrinsicMatrix()) *
                            frame->GetCameraInfo()->GetExtrinsicMatrix() * glm::vec4(v0, 1.0f))/ v0.z;
                        auto uv1 =
                            (glm::mat4(frame->GetCameraInfo()->GetIntrinsicMatrix()) *
                                frame->GetCameraInfo()->GetExtrinsicMatrix() * glm::vec4(v1, 1.0f)) / v1.z;
                        auto uv2 =
                            (glm::mat4(frame->GetCameraInfo()->GetIntrinsicMatrix()) *
                                frame->GetCameraInfo()->GetExtrinsicMatrix() * glm::vec4(v2, 1.0f)) / v2.z;

                        //pGeometry->AddTriangle(glm::vec3(uv0, 1.0f), glm::vec3(uv1, 1.0f), glm::vec3(uv2, 1.0f));

                        //pGeometry->AddUV(uv0);
                        //pGeometry->AddUV(uv1);
                        //pGeometry->AddUV(uv2);

                        pGeometry->AddTriangle(v0, v1, v2);

                        pGeometry->AddUV(uv0);
                        pGeometry->AddUV(uv1);
                        pGeometry->AddUV(uv2);
                    }
                }
            }
        }
    }

    /* Multi texture test
    {
        auto pNode = pScene->CreateSceneNode("MultiTexture");
        auto plane = gGraphics->GetGeometryPlane("MultiTexture", 1, 1, 10, 10, HePlaneType::XY);
        plane->Initialize();
        pNode->AddGeometry(plane);

        auto pMaterial = gGraphics->GetMaterialMutiTexture("MultiTexture");
        plane->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("MultiTexture", "../../res/shader/mutiTexture.vs", "../../res/shader/mutiTexture.fs");
        pMaterial->SetShader(pShader);

        auto image0 = gGraphics->GetImage("texture0", "../../res/img/awesomeface.png");
        image0->Initialize();
        auto texture0 = gGraphics->GetTexture("texture0", image0);
        texture0->Initialize();
        pMaterial->AddTexture("texture0", texture0);

        auto image1 = gGraphics->GetImage("texture1", "../../res/img/Owl.jpg");
        image1->Initialize();
        auto texture1 = gGraphics->GetTexture("texture1", image1);
        texture1->Initialize();
        pMaterial->AddTexture("texture1", texture1);
    }
    */

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
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
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

        vector<string> fileNames;
        for (size_t i = 0; i < 37; i++)
        {
            stringstream ss;
            ss << "CaptureFileName_" << i << ".png";
            fileNames.push_back(ss.str());
        }

        gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
            pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
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
