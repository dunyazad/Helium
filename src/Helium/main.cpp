#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HeShader.h"

#include "HeScene.h"
#include "HeSceneNode.h"
#include "HeCamera.h"

#include "HeGeometry.h"
#include "HeImage.h"
#include "HeTexture.h"
#include "HeMaterial.h"

using namespace ArtificialNature;

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//// Standard Headers
//#include <cstdio>
//#include <cstdlib>

const int mWidth = 1600;
const int mHeight = 800;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

HeOrthogonalCamera* pCamera = nullptr;

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

    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    HeScene scene;

    HeOrthogonalCamera camera(&scene);
    pCamera = &camera;
    scene.GetRootNode()->AddChild(&camera);
    scene.SetMainCamera(&camera);

    ////////////////////////////////////////////camera.SetPosition(glm::vec3(0.5f, 0, 0));

    HeSceneNode node(&scene);
    scene.GetRootNode()->AddChild(&node);

	HeGeometry geometry;
	geometry.Initialize();
    geometry.SetDrawingMode(HeGeometry::DrawingMode::Triangles);
	node.AddGeometry(&geometry);

	//geometry.SetDrawingMode(HeGeometry::DrawingMode::Triangles);

	geometry.AddVertex(glm::vec3(0.5f, 0.5f, 0.0f));
	geometry.AddVertex(glm::vec3(0.5f, -0.5f, 0.0f));
	geometry.AddVertex(glm::vec3(-0.5f, -0.5f, 0.0f));
	geometry.AddVertex(glm::vec3(-0.5f, 0.5f, 0.0f));

	geometry.AddIndex(0);
	geometry.AddIndex(1);
	geometry.AddIndex(3);

	geometry.AddIndex(1);
	geometry.AddIndex(2);
	geometry.AddIndex(3);

	geometry.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	geometry.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	geometry.AddUV(glm::vec2(1.0f, 1.0f));
	geometry.AddUV(glm::vec2(1.0f, 0.0f));
	geometry.AddUV(glm::vec2(0.0f, 0.0f));
	geometry.AddUV(glm::vec2(0.0f, 1.0f));

	HeMaterial material;

	HeShader shader("../../res/shader/texture.vs", "../../res/shader/texture.fs");
	//HeShader shader;
	material.SetShader(&shader);

	HeImage image("../../res/img/awesomeface.png");
	HeTexture texture;
	texture.Initialize(&image);

	material.SetTexture(&texture);

	geometry.SetHeMaterial(&material);






    //HeGeometry geometryLine;
    //geometryLine.Initialize();
    //geometryLine.SetDrawingMode(HeGeometry::DrawingMode::Wireframe);
    //node.AddGeometry(&geometryLine);

    //geometryLine.AddVertex(glm::vec3(0.5f, 0.5f, 0.0f));
    //geometryLine.AddVertex(glm::vec3(0.5f, -0.5f, 0.0f));
    //geometryLine.AddVertex(glm::vec3(-0.5f, -0.5f, 0.0f));
    //geometryLine.AddVertex(glm::vec3(-0.5f, 0.5f, 0.0f));

    //geometryLine.AddIndex(0);
    //geometryLine.AddIndex(1);
    //geometryLine.AddIndex(3);

    //geometryLine.AddIndex(1);
    //geometryLine.AddIndex(2);
    //geometryLine.AddIndex(3);

    //geometryLine.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //geometryLine.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //geometryLine.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //geometryLine.AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    //HeMaterial materialLine;

    //HeShader shaderLine("../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
    ////HeShader shader;
    //materialLine.SetShader(&shaderLine);

    //geometryLine.SetHeMaterial(&materialLine);







    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    framebuffer_size_callback(mWindow, mWidth, mHeight);

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.Update(0);
        scene.Render();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    pCamera->SetAspectRatio((float)width / (float)height);

    printf("Aspect Ratio : %f\n", (float)width / (float)height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
    float zoomFactor = pCamera->GetZoomFactor();
    zoomFactor += (float)yoffset * 0.01f;
    pCamera->SetZoomFactor(zoomFactor);
}