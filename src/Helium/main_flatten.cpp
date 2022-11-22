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
HeOrthogonalCamera* pCamera = nullptr;
//HeCameraManipulatorFlight* pCameraManipulator = nullptr;
HeCameraManipulatorOrtho* pCameraManipulator = nullptr;

HeGeometry* Flatten(const HeProject& project, HeGeometry* from, const string& name, float scale, vector<glm::vec2>& uvs, bool asBoundingBox = false);

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
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE); // Transparent Background
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

    //HeOrthogonalCamera camera("Main Camera", &scene, 0, 0, mWidth, mHeight);
    //pCamera = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
    pCamera = pScene->CreateOrthogonalCamera("Main Camera", 0, 0, mWidth, mHeight);
    //HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    pScene->SetMainCamera(pCamera);

    /*
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
    */

    {
        //auto pNode = pScene->CreateSceneNode("Mesh");
        auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\04_Fixed.stl");
        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        //pNode->AddGeometry(pGeometry);

        //auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        //pGeometry->SetMaterial(pMaterial);

        //auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        //pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
    }

    {
        HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

        auto from = gGraphics->GetGeometry("Mesh");
        //{
        //    auto pNode = pScene->CreateSceneNode("uvMesh");
        //    auto pGeometry = Flatten(from, "uvMesh", 1);
        //    //pGeometry->SetFillMode(HeGeometry::Wireframe);
        //    pNode->AddGeometry(pGeometry);
        //    auto nov = pGeometry->GetVertexCount();
        //    for (size_t i = 0; i < nov; i++)
        //    {
        //        auto& v = pGeometry->GetVertex(i);

        //        pGeometry->AddColor(glm::vec4(1, v.y / 36.0f, 0, 1));
        //    }

        //    auto pMaterial = gGraphics->GetMaterial("vertex with color");
        //    pGeometry->SetMaterial(pMaterial);

        //    auto pShader = gGraphics->GetShader("vertexColor", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
        //    pMaterial->SetShader(pShader);
        //}
        
        {
            vector<glm::vec2> uvs;

            auto pNode = pScene->CreateSceneNode("bbMesh");
            auto pGeometry = Flatten(project, from, "bbMesh", 1.0f, uvs, true);
            //pGeometry->SetFillMode(HeGeometry::Wireframe);
            pNode->AddGeometry(pGeometry);
            auto nov = pGeometry->GetVertexCount();
            for (size_t i = 0; i < nov; i++)
            {
                auto& v = pGeometry->GetVertex((int)i);

                pGeometry->AddColor(glm::vec4(1, v.y / 36.0f, 0, 1));
            }

            auto pMaterial = gGraphics->GetMaterial("vertex with color");
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("vertexColor", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
            pMaterial->SetShader(pShader);

            //for (auto& uv : uvs)
            //{
            //    cout << uv << endl;
            //}

            from->ClearUVs();
            from->SetUVs(uvs);

            //HeResourceIO::WriteOBJFile(gGraphics, from->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\WithUV.obj");
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

HeGeometry* Flatten(const HeProject& project, HeGeometry* from, const string& name, float scale, vector<glm::vec2>& uvs, bool asBoundingBox)
{
    struct FlatteningInfo
    {
        int faceIndex = -1;
        float sizeX = 0.0f;
        float sizeY = 0.0f;
        glm::vec3 v0;
        glm::vec3 v1;
        glm::vec3 v2;
        glm::vec3 fn;
        glm::vec3 fc;
        glm::vec3 normalizedUV0;
        glm::vec3 normalizedUV1;
        glm::vec3 normalizedUV2;
    };

    vector<FlatteningInfo> flatteningInfos;

    auto uvMesh = gGraphics->GetGeometryTriangleSoup(name);
    uvMesh->Initialize();

    vector<HeAABB> uvBoxes;

    const auto& frames = project.GetFrames();

    auto nof = from->GetIndexCount() / 3;
    for (size_t i = 0; i < nof; i++)
    {
        auto vi0 = from->GetIndex((int)i * 3);
        auto vi1 = from->GetIndex((int)i * 3 + 1);
        auto vi2 = from->GetIndex((int)i * 3 + 2);

        auto& v0 = from->GetVertex(vi0);
        auto& v1 = from->GetVertex(vi1);
        auto& v2 = from->GetVertex(vi2);

        auto d10 = glm::normalize(v0 - v1);
        auto d12 = glm::normalize(v2 - v1);
        auto fn = glm::normalize(glm::cross(d10, d12));
        auto fc = (v0 + v1 + v2) / 3.0f;

        auto zAxis = glm::vec3(0, 0, 1);

        auto angle = glm::angle(zAxis, fn);
        auto degree = glm::degrees(angle);

        auto axis = glm::normalize(glm::cross(fn, zAxis));
        if ((degree - 180) < FLT_EPSILON) {
            axis = glm::vec3(0, 1, 0);
            //cout << "Flipped." << endl;
        }
        auto rotation = glm::angleAxis(angle, axis);

        auto tv0 = rotation * (v0 - fc);
        auto tv1 = rotation * (v1 - fc);
        auto tv2 = rotation * (v2 - fc);

        auto stv0 = glm::vec3(tv0.x * scale, tv0.y * scale, 0);
        auto stv1 = glm::vec3(tv1.x * scale, tv1.y * scale, 0);
        auto stv2 = glm::vec3(tv2.x * scale, tv2.y * scale, 0);

        HeAABB aabb;
        aabb.Extend(stv0);
        aabb.Extend(stv1);
        aabb.Extend(stv2);

        //cout << fixed << aabb << endl;

        auto size = aabb.GetSize();
        auto center = aabb.GetCenter();
        auto aabbmin = aabb.GetMin();
        stv0.x -= aabbmin.x;
        stv0.y -= aabbmin.y;
        stv1.x -= aabbmin.x;
        stv1.y -= aabbmin.y;
        stv2.x -= aabbmin.x;
        stv2.y -= aabbmin.y;

        if (size.x == -FLT_MAX) {
            printf("Error. size.x == -FLT_MAX\n");
        }
        if (size.y == -FLT_MAX) {
            printf("Error. size.y == -FLT_MAX\n");
        }
        if (size.x == 0) {
            printf("Error. size.x == 0\n");
        }
        if (size.y == 0) {
            printf("Error. size.y == 0\n");
        }

        FlatteningInfo info;
        HeAABB infoAABB;
        infoAABB.Extend(stv0);
        infoAABB.Extend(stv1);
        infoAABB.Extend(stv2);
        info.faceIndex = (int)i;
        info.sizeX = infoAABB.GetSize().x;
        info.sizeY = infoAABB.GetSize().y;
        info.v0 = v0;
        info.v1 = v1;
        info.v2 = v2;
        info.fn = fn;
        info.fc = fc;
        info.normalizedUV0 = stv0;
        info.normalizedUV1 = stv1;
        info.normalizedUV2 = stv2;

        flatteningInfos.push_back(info);
    }

    struct FlatteningInfoLess {
        inline bool operator() (const FlatteningInfo& a, const FlatteningInfo& b) {
            return a.sizeY < b.sizeY;
        }
    };

    sort(flatteningInfos.begin(), flatteningInfos.end(), FlatteningInfoLess());

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float maxY = 0.0f;

    uvs.resize(flatteningInfos.size() * 3);

    for (auto& info : flatteningInfos)
    {
        if (info.sizeX + offsetX > 1.0f * scale) {
            offsetX = 0;
            offsetY += maxY;
            maxY = 0;
        }

        HeFrameInfo* nearestFrame = nullptr;
        float distance2 = FLT_MAX;
        for (auto& frame : frames)
        {
            auto frustum = frame->GetCameraInfo()->GetFrustum();
            vector<glm::vec3> vertices = { info.v0, info.v1, info.v2 };
            if (frustum->ContainsAll(vertices)) {

                auto ffd = glm::distance2(info.fc, frame->GetCameraInfo()->GetPosition());
                if (ffd < distance2) {
                    distance2 = ffd;
                    nearestFrame = frame;
                }
                //cout << "camera position : " << frame->GetCameraInfo()->GetPosition() << endl;
                //cout << "ffd: " << ffd << endl;
            }
        }

        if (nearestFrame != nullptr) {
            //glm::project()
            glm::vec2 textureUV0 = nearestFrame->GetCameraInfo()->WorldToUV(info.v0);
            glm::vec2 textureUV1 = nearestFrame->GetCameraInfo()->WorldToUV(info.v1);
            glm::vec2 textureUV2 = nearestFrame->GetCameraInfo()->WorldToUV(info.v2);

            if (textureUV0.x < 0 || textureUV0.y < 0) {
                cout << "Error. textureUV0: " << textureUV0 << endl;
            }
            if (textureUV1.x < 0 || textureUV1.y < 0) {
                cout << "Error. textureUV1: " << textureUV1 << endl;
            }
            if (textureUV2.x < 0 || textureUV2.y < 0) {
                cout << "Error. textureUV2: " << textureUV2 << endl;
            }

            info.normalizedUV0.x += offsetX;
            info.normalizedUV0.y += offsetY;
            info.normalizedUV1.x += offsetX;
            info.normalizedUV1.y += offsetY;
            info.normalizedUV2.x += offsetX;
            info.normalizedUV2.y += offsetY;

            if (info.normalizedUV0.x < 0 || info.normalizedUV0.y < 0) {
                cout << "Error. stv0: " << info.normalizedUV0 << endl;
            }
            if (info.normalizedUV1.x < 0 || info.normalizedUV1.y < 0) {
                cout << "Error. stv1: " << info.normalizedUV1 << endl;
            }
            if (info.normalizedUV2.x < 0 || info.normalizedUV2.y < 0) {
                cout << "Error. stv2: " << info.normalizedUV2 << endl;
            }

            auto fi = info.faceIndex;
            auto uvi0 = fi * 3;
            auto uvi1 = fi * 3 + 1;
            auto uvi2 = fi * 3 + 2;
            uvs[uvi0].x = info.normalizedUV0.x;
            uvs[uvi0].y = info.normalizedUV0.y;
            uvs[uvi1].x = info.normalizedUV1.x;
            uvs[uvi1].y = info.normalizedUV1.y;
            uvs[uvi2].x = info.normalizedUV2.x;
            uvs[uvi2].y = info.normalizedUV2.y;

            if (asBoundingBox == false)
            {
                uvMesh->AddTriangle(info.normalizedUV0, info.normalizedUV1, info.normalizedUV2);
            }

            offsetX += info.sizeX;
            maxY = maxY > info.sizeY ? maxY : info.sizeY;

            if (offsetX < 0) {
                cout << "Error. offsetX < 0" << endl;
            }

            if (asBoundingBox)
            {
                HeAABB tempAABB;
                //tempAABB.Extend(textureUV0);
                //tempAABB.Extend(textureUV0);
                //tempAABB.Extend(textureUV0);
                tempAABB.Extend(info.normalizedUV0);
                tempAABB.Extend(info.normalizedUV1);
                tempAABB.Extend(info.normalizedUV2);

                auto tamin = tempAABB.GetMin();
                auto tamax = tempAABB.GetMax();

                uvMesh->AddTriangle(
                    glm::vec3(tamin.x, tamin.y, 0),
                    glm::vec3(tamin.x, tamax.y, 0),
                    glm::vec3(tamax.x, tamax.y, 0));
                uvMesh->AddTriangle(
                    glm::vec3(tamin.x, tamin.y, 0),
                    glm::vec3(tamax.x, tamax.y, 0),
                    glm::vec3(tamax.x, tamin.y, 0));
            }
        }
        else
        {
            cout << "Error. nearest frame is null" << endl;
        }
    }

    return uvMesh;
}
