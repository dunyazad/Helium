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

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

//// Standard Headers
//#include <cstdio>
//#include <cstdlib>

const int mWidth = 1024;
const int mHeight = 1024;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

HeGraphics* gGraphics = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorOrbital* pCameraManipulator = nullptr;
HeSceneNode* pPlane = nullptr;

HeGeometry* Flatten(HeGeometry* from, const string& name, bool asBoundingBox = false);

int main(int argc, char* argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

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




    Helium helium("helium");
    gGraphics = helium.GetGraphics();

    auto pScene = helium.GetScene("Default Scene");

    //HeOrthogonalCamera camera("Main Camera", &scene, 0, 0, mWidth, mHeight);
    pCamera = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
    HeCameraManipulatorOrbital manipulator(pCamera);
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
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
    }

    {
        auto from = gGraphics->GetGeometry("Mesh");
        {
            auto pNode = pScene->CreateSceneNode("uvMesh");
            auto pGeometry = Flatten(from, "uvMesh");
            pGeometry->SetFillMode(HeGeometry::Wireframe);
            pNode->AddGeometry(pGeometry);
            auto nov = pGeometry->GetVertexCount();
            for (size_t i = 0; i < nov; i++)
            {
                pGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            }

            auto pMaterial = gGraphics->GetMaterial("vertex with color");
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("vertexColor", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
            pMaterial->SetShader(pShader);
        }
        
        {
            auto pNode = pScene->CreateSceneNode("bbMesh");
            auto pGeometry = Flatten(from, "bbMesh", true);
            pGeometry->SetFillMode(HeGeometry::Wireframe);
            pNode->AddGeometry(pGeometry);
            auto nov = pGeometry->GetVertexCount();
            for (size_t i = 0; i < nov; i++)
            {
                pGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            }

            auto pMaterial = gGraphics->GetMaterial("vertex with color");
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("vertexColor", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
            pMaterial->SetShader(pShader);
        }
    }

    {
        auto pNode = pScene->CreateSceneNode("MultiTexture");
        auto plane = gGraphics->GetGeometryPlane("MultiTexture", 1, 1, 10, 10, HePlaneType::XY);
        plane->Initialize();
        pNode->AddGeometry(plane);

        auto pMaterial = gGraphics->GetMaterial("MultiTexture");
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

        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
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
    pCameraManipulator->OnKey(window, key, scancode, action, mods);

    if (key == '1' && action == 0)
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
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    pCameraManipulator->OnMousePosition(window, xpos, ypos);

    //if (firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = false;
    //}

    //float xoffset = xpos - lastX;
    //float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    //lastX = xpos;
    //lastY = ypos;

    //float sensitivity = 0.1f; // change this value to your liking
    //xoffset *= sensitivity;
    //yoffset *= sensitivity;

    //yaw += xoffset;
    //pitch += yoffset;

    //// make sure that when pitch is out of bounds, screen doesn't get flipped
    //if (pitch > 89.0f)
    //    pitch = 89.0f;
    //if (pitch < -89.0f)
    //    pitch = -89.0f;

    //glm::vec3 front;
    //front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //front.y = sin(glm::radians(pitch));
    //front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //cameraFront = glm::normalize(front);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    pCameraManipulator->OnMouseButton(window, button, action, mods);
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    pCameraManipulator->OnWheel(window, xoffset, yoffset);
}

HeGeometry* Flatten(HeGeometry* from, const string& name, bool asBoundingBox)
{
    auto uvMesh = gGraphics->GetGeometryTriangleSoup(name);
    uvMesh->Initialize();

    vector<HeAABB> uvBoxes;

    float offsetX = 0.0f;
    float offsetY = 0.0f;
    float maxY = 0.0f;

    auto nof = from->GetIndexCount() / 3;
    for (size_t i = 0; i < nof; i++)
    {
        auto vi0 = from->GetIndex(i * 3);
        auto vi1 = from->GetIndex(i * 3 + 1);
        auto vi2 = from->GetIndex(i * 3 + 2);

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

        auto stv0 = glm::vec3(tv0.x * 100, tv0.y * 100, 0);
        auto stv1 = glm::vec3(tv1.x * 100, tv1.y * 100, 0);
        auto stv2 = glm::vec3(tv2.x * 100, tv2.y * 100, 0);

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

        if (size.x + offsetX > 1024) {
            offsetX = 0;
            offsetY += maxY;
            maxY = 0;
        }

        stv0.x += offsetX;
        stv0.y += offsetY;
        stv1.x += offsetX;
        stv1.y += offsetY;
        stv2.x += offsetX;
        stv2.y += offsetY;

        if (stv0.x < 0 || stv0.y < 0) {
            cout << "Error. stv0: " << stv0 << endl;
        }
        if (stv1.x < 0 || stv1.y < 0) {
            cout << "Error. stv1: " << stv1 << endl;
        }
        if (stv2.x < 0 || stv2.y < 0) {
            cout << "Error. stv2: " << stv2 << endl;
        }

        if (asBoundingBox == false)
        {
            uvMesh->AddTriangle(stv0, stv1, stv2);
        }

        offsetX += size.x;
        maxY = maxY < size.y ? size.y : maxY;

        if (offsetX < 0)
        {
            printf("Error. offsetX < 0\n");
        }

        if (asBoundingBox)
        {
            HeAABB tempAABB;
            tempAABB.Extend(stv0);
            tempAABB.Extend(stv1);
            tempAABB.Extend(stv2);

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

    return uvMesh;
}