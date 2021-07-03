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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorObital* pCameraManipulator = nullptr;

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
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    HeScene scene;

    //HeOrthogonalCamera camera(&scene, 0, 0, mWidth, mHeight);
    HePerspectiveCamera camera(&scene, 0, 0, mWidth, mHeight);
    pCamera = &camera;
    HeCameraManipulatorObital manipulator(&camera);
    pCameraManipulator = &manipulator;

    scene.GetRootNode()->AddChild(&camera);
    scene.SetMainCamera(&camera);

    ////////////////////////////////////////////camera.SetPosition(glm::vec3(0.5f, 0, 0));


    HeSceneNode node(&scene);
    scene.GetRootNode()->AddChild(&node);


    HeThickLines geometryLine;
    geometryLine.Initialize();
    geometryLine.SetThickness(5);
    node.AddGeometry(&geometryLine);

    geometryLine.AddVertex(glm::vec3(-0.25f, -0.25f, 0.0f));
    geometryLine.AddVertex(glm::vec3(0.0f, 0.0f, 0.0f));
    geometryLine.AddVertex(glm::vec3(-0.25f, 0.0f, 0.0f));
    geometryLine.AddVertex(glm::vec3(0.25f, 0.25f, 0.0f));
    geometryLine.AddVertex(glm::vec3(0.5f, 0.25f, 0.0f));
    geometryLine.AddVertex(glm::vec3(0.5f, 0.5f, 0.0f));

    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));
    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));
    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));
    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));
    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));
    geometryLine.AddColor(glm::vec4(1, 1, 1, 1));

    HeMaterial materialLine;

    HeShader shaderLine("../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
    materialLine.SetShader(&shaderLine);

    geometryLine.SetHeMaterial(&materialLine);


    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    framebuffer_size_callback(mWindow, mWidth, mHeight);









    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {

        // Background Fill Color
        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.Update(0);
        scene.Render();



        ////////////////ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        // Flip Buffers and Draw
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    pCameraManipulator->OnMouse(window, xpos, ypos);

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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    pCameraManipulator->OnWheel(window, xoffset, yoffset);

    //float zoomFactor = pCamera->GetZoomFactor();
    //if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    //    zoomFactor += (float)yoffset * zoomFactor * 0.1f;
    //else
    //    zoomFactor += (float)yoffset * zoomFactor * 0.01f;

    //pCamera->SetZoomFactor(zoomFactor);
}