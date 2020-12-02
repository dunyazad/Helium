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
#include "HeThickLines.h"

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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

HePerspectiveCamera* pCamera = nullptr;

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

    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("../../res/font/malgun.ttf", 16.0f);
        auto font = io.Fonts->AddFontFromFileTTF("../../res/font/malgun.ttf", 16.0f, 0, io.Fonts->GetGlyphRangesKorean());
        //ImGui::PushFont(font);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != NULL);
    }

    HeScene scene;

    //HeOrthogonalCamera camera(&scene, 0, 0, mWidth, mHeight);
    HePerspectiveCamera camera(&scene, 0, 0, mWidth, mHeight);
    pCamera = &camera;
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




    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);






    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {

        ////////////////ImGui_ImplOpenGL3_NewFrame();
        ////////////////ImGui_ImplGlfw_NewFrame();
        ////////////////ImGui::NewFrame();

        ////////////////ImGui::ShowDemoWindow(&show_demo_window);

        ////////////////{
        ////////////////    static float f = 0.0f;
        ////////////////    static int counter = 0;

        ////////////////    ImGui::Begin("ÇÑ±Û·Î Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ////////////////    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ////////////////    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ////////////////    ImGui::Checkbox("Another Window", &show_another_window);

        ////////////////    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ////////////////    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        ////////////////    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        ////////////////        counter++;
        ////////////////    ImGui::SameLine();
        ////////////////    ImGui::Text("counter = %d", counter);

        ////////////////    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ////////////////    ImGui::End();
        ////////////////}

        //////////////////// 3. Show another simple window.
        //////////////////if (show_another_window)
        //////////////////{
        //////////////////    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //////////////////    ImGui::Text("Hello from another window!");
        //////////////////    if (ImGui::Button("Close Me"))
        //////////////////        show_another_window = false;
        //////////////////    ImGui::End();
        //////////////////}

        ////////////////// Rendering
        ////////////////ImGui::Render();


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
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        auto& p = pCamera->GetPosition();
        auto& t = pCamera->GetTargetPosition();

        float dist = glm::distance(t, p);

        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0, 0, -1 * dist / 10));
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        auto& p = pCamera->GetPosition();
        auto& t = pCamera->GetTargetPosition();

        float dist = glm::distance(t, p);

        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0, 0, 1 * dist / 10));
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(-0.1, 0, 0));
        pCamera->SetTargetPosition(pCamera->GetTargetPosition() + glm::vec3(-0.1, 0, 0));
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0.1, 0, 0));
        pCamera->SetTargetPosition(pCamera->GetTargetPosition() + glm::vec3(0.1, 0, 0));
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0, -0.1, 0));
        pCamera->SetTargetPosition(pCamera->GetTargetPosition() + glm::vec3(0, -0.1, 0));
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        pCamera->SetPosition(pCamera->GetPosition() + glm::vec3(0, 0.1, 0));
        pCamera->SetTargetPosition(pCamera->GetTargetPosition() + glm::vec3(0, 0.1, 0));
    }
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        zoomFactor += (float)yoffset * zoomFactor * 0.1f;
    else
        zoomFactor += (float)yoffset * zoomFactor * 0.01f;

    pCamera->SetZoomFactor(zoomFactor);
}