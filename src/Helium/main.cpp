#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "HeShader.h"

#include "HeScene.h"
#include "HeSceneNode.h"

#include "HeGeometry.h"
#include "HeImage.h"
#include "HeTexture.h"

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

const int mWidth = 1280;
const int mHeight = 800;

int main(int argc, char* argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    HeScene scene;

    HeSceneNode node(&scene);
    scene.GetRootNode()->AddChild(&node);

    HeGeometry geometry;
    geometry.Initialize();
    node.SetGeometry(&geometry);
    geometry.AddVertex(glm::vec3( 0.5f,  0.5f, 0.0f));
    geometry.AddVertex(glm::vec3( 0.5f, -0.5f, 0.0f));
    geometry.AddVertex(glm::vec3(-0.5f, -0.5f, 0.0f));
    geometry.AddVertex(glm::vec3(-0.5f,  0.5f, 0.0f));

    geometry.AddIndex(0);
    geometry.AddIndex(1);
    geometry.AddIndex(3);

    geometry.AddIndex(1);
    geometry.AddIndex(2);
    geometry.AddIndex(3);

    geometry.Upload();

    HeShader shader;
    geometry.SetShader(&shader);

    HeImage image("../../res/img/awesomeface.png");
    HeTexture texture;
    texture.Initialize(&image);

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
