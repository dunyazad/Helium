#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Helium.h"

using namespace ArtificialNature;

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


//// Standard Headers
//#include <cstdio>
//#include <cstdlib>

const int mWidth = 1600;
const int mHeight = 800;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorOrbital* pCameraManipulator = nullptr;

int main(int argc, char* argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
    //glfwSwapInterval(1); // Enable vsync

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetCursorPosCallback(mWindow, mouse_position_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
    glfwSetScrollCallback(mWindow, mouse_wheel_callback);

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


    Helium helium("helium");


    glEnable(GL_DEPTH_TEST);

    auto pScene = helium.GetScene("Default Scene");

    //HeOrthogonalCamera camera("Main Camera", &scene, 0, 0, mWidth, mHeight);
    pCamera = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
    HeCameraManipulatorOrbital manipulator(pCamera);
    pCameraManipulator = &manipulator;
    pScene->SetMainCamera(pCamera);

    {
        auto pNode = pScene->CreateSceneNode("Cube Node");

        float angle = 0.0;
        auto callback = pNode->AddOnPreupdate(
            [&angle](HeSceneNode* pNode, float dt) {
                angle += dt * 0.001f;
                pNode->SetLocalRotation(glm::angleAxis((float)angle, glm::vec3(0, 1, 0)));
            });

        auto pGeometry = helium.GetGraphics()->GetGeometry("Cube");
        pNode->AddGeometry(pGeometry);
        pGeometry->Initialize();

        #pragma region [Cube]
#pragma region [Cube Vertices]
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));

        pGeometry->AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
        pGeometry->AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));
#pragma endregion

#pragma region [Cube Colors]
        pGeometry->AddColor(glm::vec4(0.583f, 0.771f, 0.014f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.609f, 0.115f, 0.436f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.327f, 0.483f, 0.844f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.822f, 0.569f, 0.201f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.435f, 0.602f, 0.223f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.310f, 0.747f, 0.185f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.597f, 0.770f, 0.761f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.559f, 0.436f, 0.730f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.359f, 0.583f, 0.152f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.483f, 0.596f, 0.789f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.559f, 0.861f, 0.639f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.195f, 0.548f, 0.859f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.014f, 0.184f, 0.576f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.771f, 0.328f, 0.970f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.406f, 0.615f, 0.116f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.676f, 0.977f, 0.133f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.971f, 0.572f, 0.833f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.140f, 0.616f, 0.489f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.997f, 0.513f, 0.064f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.945f, 0.719f, 0.592f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.543f, 0.021f, 0.978f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.279f, 0.317f, 0.505f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.167f, 0.620f, 0.077f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.347f, 0.857f, 0.137f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.055f, 0.953f, 0.042f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.714f, 0.505f, 0.345f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.783f, 0.290f, 0.734f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.722f, 0.645f, 0.174f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.302f, 0.455f, 0.848f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.225f, 0.587f, 0.040f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.517f, 0.713f, 0.338f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.053f, 0.959f, 0.120f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.393f, 0.621f, 0.362f, 1.0f));

        pGeometry->AddColor(glm::vec4(0.673f, 0.211f, 0.457f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.820f, 0.883f, 0.371f, 1.0f));
        pGeometry->AddColor(glm::vec4(0.982f, 0.099f, 0.879f, 1.0f));
#pragma endregion

#pragma region [Cube UVs]
        pGeometry->AddUV(glm::vec2(0.000059f, 1.0f - 0.000004f));
        pGeometry->AddUV(glm::vec2(0.000103f, 1.0f - 0.336048f));
        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));

        pGeometry->AddUV(glm::vec2(1.000023f, 1.0f - 0.000013f));
        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));
        pGeometry->AddUV(glm::vec2(0.999958f, 1.0f - 0.336064f));

        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));
        pGeometry->AddUV(glm::vec2(0.336024f, 1.0f - 0.671877f));
        pGeometry->AddUV(glm::vec2(0.667969f, 1.0f - 0.671889f));

        pGeometry->AddUV(glm::vec2(1.000023f, 1.0f - 0.000013f));
        pGeometry->AddUV(glm::vec2(0.668104f, 1.0f - 0.000013f));
        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));

        pGeometry->AddUV(glm::vec2(0.000059f, 1.0f - 0.000004f));
        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));
        pGeometry->AddUV(glm::vec2(0.336098f, 1.0f - 0.000071f));

        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));
        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));
        pGeometry->AddUV(glm::vec2(0.336024f, 1.0f - 0.671877f));

        pGeometry->AddUV(glm::vec2(1.000004f, 1.0f - 0.671847f));
        pGeometry->AddUV(glm::vec2(0.999958f, 1.0f - 0.336064f));
        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));

        pGeometry->AddUV(glm::vec2(0.668104f, 1.0f - 0.000013f));
        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));
        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));

        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));
        pGeometry->AddUV(glm::vec2(0.668104f, 1.0f - 0.000013f));
        pGeometry->AddUV(glm::vec2(0.336098f, 1.0f - 0.000071f));

        pGeometry->AddUV(glm::vec2(0.000103f, 1.0f - 0.336048f));
        pGeometry->AddUV(glm::vec2(0.000004f, 1.0f - 0.671870f));
        pGeometry->AddUV(glm::vec2(0.336024f, 1.0f - 0.671877f));

        pGeometry->AddUV(glm::vec2(0.000103f, 1.0f - 0.336048f));
        pGeometry->AddUV(glm::vec2(0.336024f, 1.0f - 0.671877f));
        pGeometry->AddUV(glm::vec2(0.335973f, 1.0f - 0.335903f));

        pGeometry->AddUV(glm::vec2(0.667969f, 1.0f - 0.671889f));
        pGeometry->AddUV(glm::vec2(1.000004f, 1.0f - 0.671847f));
        pGeometry->AddUV(glm::vec2(0.667979f, 1.0f - 0.335851f));
#pragma endregion

#pragma region [Cube Indices]
        for (GLuint i = 0; i < 36; i++)
        {
            pGeometry->AddIndex(i);
        }
#pragma endregion

#pragma region [Cube Material]
        auto pMaterial = helium.GetGraphics()->GetMaterial("Cube Material");

        // Colored
        //HeShader shader("../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
        //material.SetShader(&shader);

        // Textured
        auto pShader = helium.GetGraphics()->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
        pMaterial->SetShader(pShader);

        auto pImage = helium.GetGraphics()->GetImage("dice image", "../../res/img/dice.png");
        auto pTexture = helium.GetGraphics()->GetTexture("dice texture", pImage);
        pTexture->Initialize();

        pMaterial->SetTexture(pTexture);

        pGeometry->SetMaterial(pMaterial);
#pragma endregion
#pragma endregion
    }

    {
        auto pNode = pScene->CreateSceneNode("Gizmo Node");
        
        #pragma region [Lines]
        auto pLines = helium.GetGraphics()->GetGeometryThickLines("Gizmo");
        pLines->Initialize();
        pLines->SetThickness(1);
        pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pNode->AddGeometry(pLines);

        pLines->AddVertex(glm::vec3(-100, 0, 0));
        pLines->AddVertex(glm::vec3(100, 0, 0));
        pLines->AddVertex(glm::vec3(0, -100, 0));
        pLines->AddVertex(glm::vec3(0, 100, 0));
        pLines->AddVertex(glm::vec3(0, 0, -100));
        pLines->AddVertex(glm::vec3(0, 0, 100));

        pLines->AddColor(glm::vec4(1, 0, 0, 1));
        pLines->AddColor(glm::vec4(1, 0, 0, 1));
        pLines->AddColor(glm::vec4(0, 1, 0, 1));
        pLines->AddColor(glm::vec4(0, 1, 0, 1));
        pLines->AddColor(glm::vec4(0, 0, 1, 1));
        pLines->AddColor(glm::vec4(0, 0, 1, 1));

        auto pMaterial = helium.GetGraphics()->GetMaterial("Gizmo Materials");
        
        auto pShader = helium.GetGraphics()->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLines->SetMaterial(pMaterial);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        #pragma endregion
    }

    {
        auto pNode = pScene->CreateSceneNode("Plane");
        auto pGeometry = helium.GetGraphics()->GetGeometryPlane("Plane.Geometry", 100, 100, 100, 100, HePlaneType::XY);
        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = helium.GetGraphics()->GetMaterial("Plane Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = helium.GetGraphics()->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
        pMaterial->SetShader(pShader);

        auto pImage = helium.GetGraphics()->GetImage("owl image", "../../res/img/Owl.jpg");
     
        auto pTexture = helium.GetGraphics()->GetTexture("owl texture", pImage);
        pTexture->Initialize();
        pMaterial->SetTexture(pTexture);
    }

    framebuffer_size_callback(mWindow, mWidth, mHeight);

    auto lastTime = HeTime::Now();
    while (glfwWindowShouldClose(mWindow) == false) {
        auto delta = HeTime::DeltaMili(lastTime);
        lastTime = HeTime::Now();

        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pScene->Update((float)delta);
        pScene->Render();

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
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    pCameraManipulator->OnKey(window, key, scancode, action, mods);
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