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


    {
        auto pNode = gScene->CreateSceneNode("Mesh");
        auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\04_Fixed.stl");
        //auto pGeometry = HeResourceIO::ReadOBJFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\01_MeshFromRGBD.obj");

        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        //pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //pGeometry->ClearNormals();
        
        //auto start = HeTime::Now();

        //ComputeFaceNormals((HeGeometryTriangleSoup*)pGeometry);
        //((HeGeometryTriangleSoup*)pGeometry)->ComputeFaceNormals();

        //pGeometry->RayIntersect(glm::vec3(1, 1, 1), -glm::normalize(glm::vec3(1, 1, 1)));
        //RayIntersect((HeGeometryTriangleSoup*)pGeometry, glm::vec3(1, 1, 1), -glm::normalize(glm::vec3(1, 1, 1)));

        //cout << HeTime::DeltaMili(start) << " miliseconds" << endl;

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Temp\\TestOBJ.obj");
    }

 /*   {
        auto pGeometry = gGraphics->GetGeometry("Mesh");

        auto pNode = gScene->CreateSceneNode("Debugging");

        auto pLines = gGraphics->GetGeometryThickLines("Debugging");
        pLines->Initialize();
        pLines->SetThickness(1);
        pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pNode->AddGeometry(pLines);

        auto pMaterial = gGraphics->GetMaterial("thick lines");

        auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLines->SetMaterial(pMaterial);

        auto nof = pGeometry->GetFaceCount();
        for (size_t fi = 0; fi < nof; fi++)
        {
            auto vi0 = pGeometry->GetIndex(fi * 3);
            auto vi1 = pGeometry->GetIndex(fi * 3 + 1);
            auto vi2 = pGeometry->GetIndex(fi * 3 + 2);

            const auto& v0 = pGeometry->GetVertex(vi0);
            const auto& v1 = pGeometry->GetVertex(vi1);
            const auto& v2 = pGeometry->GetVertex(vi2);

            pLines->AddVertex(v0);
            pLines->AddVertex(v1);
            pLines->AddVertex(v1);
            pLines->AddVertex(v2);
            pLines->AddVertex(v2);
            pLines->AddVertex(v0);

            pLines->AddColor(glm::vec4(1, 0, 1, 1));
            pLines->AddColor(glm::vec4(1, 0, 1, 1));
            pLines->AddColor(glm::vec4(1, 0, 1, 1));
            pLines->AddColor(glm::vec4(1, 0, 1, 1));
            pLines->AddColor(glm::vec4(1, 0, 1, 1));
            pLines->AddColor(glm::vec4(1, 0, 1, 1));
        }
    }*/

    
    {
        HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

        vector<const HeCameraInfo*> cameraInfos;
        vector<HeGeometryTriangleSoup*> frameGeometries;
        for (auto& frame : project.GetFrames())
        {
            cameraInfos.push_back(frame->GetCameraInfo());

            auto id = format("frame_{}", frame->GetFrameIndex());
            auto pNode = gScene->CreateSceneNode(id);
            onoff.AddSceneNode(pNode);
            auto pGeometry = gGraphics->GetGeometryTriangleSoup(id);
            pGeometry->Initialize();
            frameGeometries.push_back(pGeometry);
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterialSingleTexture(id);
            pGeometry->SetMaterial(pMaterial);
         
            auto pShader = gGraphics->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
            pMaterial->SetShader(pShader);

            auto image = frame->LoadColorImage(gGraphics);
            auto pTexture = gGraphics->GetTexture(id, image);
            pTexture->Initialize();
            pMaterial->SetTexture(pTexture);
        }

        auto start = HeTime::Now();
        ComputeOcclusionInfo((HeGeometryTriangleSoup*)(gGraphics->GetGeometry("Mesh")), cameraInfos);
        cout << HeTime::DeltaMili(start) << " miliseconds" << endl;

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

            HeFrameInfo* perpendicularFrame = nullptr;
            float dotBetweenCameraAndFaceNormal = FLT_MAX;

            HeFrameInfo* nearUVCenterFrame = nullptr;
            float uvCenterDistance2 = FLT_MAX;
            for (auto& frame : project.GetFrames())
            {
                auto cameraInfo = frame->GetCameraInfo();
                auto cameraFront = glm::vec3(cameraInfo->GetViewMatrix()[3]);
                auto frustum = cameraInfo->GetFrustum();


                if (frustum->ContainsAll(v0, v1, v2))
                {
                    //if ((mesh->LineIntersect(cameraInfo->GetPosition(), v0).size() == 0) &&
                    //    (mesh->LineIntersect(cameraInfo->GetPosition(), v1).size() == 0) &&
                    //    (mesh->LineIntersect(cameraInfo->GetPosition(), v2).size() == 0))
                    //{
                        auto ffd = glm::distance2(fc, frame->GetCameraInfo()->GetPosition());
                        if (ffd < nearestDistance2) {
                            nearestDistance2 = ffd;
                            nearestFrame = frame;
                        }

                        auto dot = glm::dot(cameraFront, fn);
                        if (dotBetweenCameraAndFaceNormal > dot) {
                            dotBetweenCameraAndFaceNormal = dot;
                            perpendicularFrame = frame;
                        }

                        auto uv0 = cameraInfo->WorldToUV(v0);
                        auto uv1 = cameraInfo->WorldToUV(v1);
                        auto uv2 = cameraInfo->WorldToUV(v2);

                        auto d0 = glm::distance2(uv0, glm::vec2(0.5f, 0.5f));
                        auto d1 = glm::distance2(uv0, glm::vec2(0.5f, 0.5f));
                        auto d2 = glm::distance2(uv0, glm::vec2(0.5f, 0.5f));
                        auto meand = (d0 + d1 + d2) / 3;

                        if (uvCenterDistance2 > meand) {
                            uvCenterDistance2 = meand;
                            nearUVCenterFrame = frame;
                        }
                    //}
                }
            }

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

        //HeResourceIO::WriteOBJFile(gGraphics, frameGeometries, "D:\\temp\\test.obj");
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
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //gScene->GetSceneNode("Plane")->SetActive(true);

        gScene->Update((float)delta);
        gScene->Render();

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
