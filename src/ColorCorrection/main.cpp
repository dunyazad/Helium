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

class HeVisualDebugger
{
public:
    HeVisualDebugger(Helium& helium)
        : helium(helium)
    {
        scene = helium.GetScene("Default Scene");
        graphics = helium.GetGraphics();

        solidSceneNode = scene->CreateSceneNode("Visual Debugger.SolidSceneNode");
        solidGeometry = graphics->GetGeometryTriangleSoup("Visual Debugger.SolidGeometry");
        solidGeometry->Initialize();
        solidSceneNode->AddGeometry(solidGeometry);
        solidMaterial = gGraphics->GetMaterial("Visual Debugger.SolidMaterial");
        solidShader = gGraphics->GetShader("Visual Debugger.SolidShader", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
        solidMaterial->SetShader(solidShader);
        solidGeometry->SetMaterial(solidMaterial);

        lineSceneNode = scene->CreateSceneNode("Visual Debugger.LineSceneNode");
        lineGeometry = graphics->GetGeometryThickLines("Visual Debugger.LineGeometry");
        lineGeometry->Initialize();
        lineSceneNode->AddGeometry(lineGeometry);
        lineMaterial = gGraphics->GetMaterial("Visual Debugger.LineMaterial");
        lineShader = gGraphics->GetShader("Visual Debugger.LineShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        lineMaterial->SetShader(lineShader);
        lineGeometry->SetMaterial(lineMaterial);
        lineGeometry->SetThickness(1);
        lineGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

        axisSceneNode = scene->CreateSceneNode("Visual Debugger.AxisSceneNode");
        axisGeometry = graphics->GetGeometryThickLines("Visual Debugger.AxisGeometry");
        axisGeometry->Initialize();
        axisSceneNode->AddGeometry(axisGeometry);
        axisMaterial = gGraphics->GetMaterial("Visual Debugger.AxisMaterial");
        axisShader = gGraphics->GetShader("Visual Debugger.AxisShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        axisMaterial->SetShader(axisShader);
        axisGeometry->SetMaterial(axisMaterial);
        axisGeometry->SetThickness(1);
        axisGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

        axisGeometry->AddVertex(glm::vec3(-1024, 0, 0));
        axisGeometry->AddVertex(glm::vec3(1024, 0, 0));
        axisGeometry->AddVertex(glm::vec3(0, -1024, 0));
        axisGeometry->AddVertex(glm::vec3(0, 1024, 0));
        axisGeometry->AddVertex(glm::vec3(0, 0, -1024));
        axisGeometry->AddVertex(glm::vec3(0, 0, 1024));

        axisGeometry->AddColor(glm::vec4(1, 0, 0, 1));
        axisGeometry->AddColor(glm::vec4(1, 0, 0, 1));
        axisGeometry->AddColor(glm::vec4(0, 1, 0, 1));
        axisGeometry->AddColor(glm::vec4(0, 1, 0, 1));
        axisGeometry->AddColor(glm::vec4(0, 0, 1, 1));
        axisGeometry->AddColor(glm::vec4(0, 0, 1, 1));

        //{
        //	auto pNode = gScene->CreateSceneNode("Grid");
        //	auto pGeometry = gGraphics->GetGeometryPlane("Grid.Geometry", 2, 2, 1, 1, HePlaneType::XY);
        //	pGeometry->Initialize();
        //	pNode->AddGeometry(pGeometry);
        //	auto pMaterial = gGraphics->GetMaterial("Grid.Material");
        //	pGeometry->SetMaterial(pMaterial);
        //	auto pShader = gGraphics->GetShader("Grid.Shader", "../../res/shader/grid.vs", "../../res/shader/grid.fs");
        //	pMaterial->SetShader(pShader);
        //}
    }

    void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
    {
        AddTriangle(v0, v1, v2, glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1));
    }

    void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color0, const glm::vec4& color1, const glm::vec4& color2)
    {
        int vertexCount = (int)lineGeometry->GetVertexCount();

        lineGeometry->AddVertex(v0);
        lineGeometry->AddVertex(v1);
        lineGeometry->AddVertex(v1);
        lineGeometry->AddVertex(v2);
        lineGeometry->AddVertex(v2);
        lineGeometry->AddVertex(v0);

        lineGeometry->AddIndex(vertexCount);
        lineGeometry->AddIndex(vertexCount + 1);
        lineGeometry->AddIndex(vertexCount + 2);
        lineGeometry->AddIndex(vertexCount + 3);
        lineGeometry->AddIndex(vertexCount + 4);
        lineGeometry->AddIndex(vertexCount + 5);

        lineGeometry->AddColor(color0);
        lineGeometry->AddColor(color1);
        lineGeometry->AddColor(color1);
        lineGeometry->AddColor(color2);
        lineGeometry->AddColor(color2);
        lineGeometry->AddColor(color0);

        solidGeometry->AddTriangle(v0, v1, v2, color0, color1, color2);
    }

    void AddLine(const glm::vec3& v0, const glm::vec3& v1)
    {
        AddLine(v0, v1, glm::vec4(1, 1, 1, 1), glm::vec4(1, 1, 1, 1));
    }

    void AddLine(const glm::vec3& v0, const glm::vec3& v1, const glm::vec4& color0, const glm::vec4& color1)
    {
        int vertexCount = (int)lineGeometry->GetVertexCount();

        lineGeometry->AddVertex(v0);
        lineGeometry->AddVertex(v1);

        lineGeometry->AddIndex(vertexCount);
        lineGeometry->AddIndex(vertexCount + 1);

        lineGeometry->AddColor(color0);
        lineGeometry->AddColor(color1);
    }

    void AddBox(const glm::vec3& bmin, const glm::vec3& bmax)
    {
        AddBox(bmin, bmax, glm::vec4(1, 1, 1, 1));
    }

    void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const glm::vec4& color)
    {
        glm::vec3 llb(bmin.x, bmin.y, bmin.z);
        glm::vec3 rlb(bmax.x, bmin.y, bmin.z);
        glm::vec3 lub(bmin.x, bmax.y, bmin.z);
        glm::vec3 rub(bmax.x, bmax.y, bmin.z);
        glm::vec3 llf(bmin.x, bmin.y, bmax.z);
        glm::vec3 rlf(bmax.x, bmin.y, bmax.z);
        glm::vec3 luf(bmin.x, bmax.y, bmax.z);
        glm::vec3 ruf(bmax.x, bmax.y, bmax.z);

        int vertexCount = (int)lineGeometry->GetVertexCount();

        lineGeometry->AddVertex(llf);
        lineGeometry->AddVertex(rlf);
        lineGeometry->AddVertex(llb);
        lineGeometry->AddVertex(rlb);
        lineGeometry->AddVertex(llf);
        lineGeometry->AddVertex(llb);
        lineGeometry->AddVertex(rlf);
        lineGeometry->AddVertex(rlb);

        lineGeometry->AddVertex(luf);
        lineGeometry->AddVertex(ruf);
        lineGeometry->AddVertex(lub);
        lineGeometry->AddVertex(rub);
        lineGeometry->AddVertex(luf);
        lineGeometry->AddVertex(lub);
        lineGeometry->AddVertex(ruf);
        lineGeometry->AddVertex(rub);

        lineGeometry->AddVertex(llf);
        lineGeometry->AddVertex(luf);
        lineGeometry->AddVertex(rlf);
        lineGeometry->AddVertex(ruf);
        lineGeometry->AddVertex(llb);
        lineGeometry->AddVertex(lub);
        lineGeometry->AddVertex(rlb);
        lineGeometry->AddVertex(rub);

        for (int i = vertexCount; i < (int)lineGeometry->GetVertexCount(); i++)
        {
            lineGeometry->AddIndex(i);
            lineGeometry->AddColor(color);
        }
    }

protected:
    Helium& helium;
    HeScene* scene = nullptr;
    HeGraphics* graphics = nullptr;

    HeSceneNode* solidSceneNode = nullptr;
    HeGeometryTriangleSoup* solidGeometry = nullptr;
    HeMaterial* solidMaterial = nullptr;
    HeShader* solidShader = nullptr;

    HeSceneNode* lineSceneNode = nullptr;
    HeGeometryThickLines* lineGeometry = nullptr;
    HeMaterial* lineMaterial = nullptr;
    HeShader* lineShader = nullptr;

    HeSceneNode* axisSceneNode = nullptr;
    HeGeometryThickLines* axisGeometry = nullptr;
    HeMaterial* axisMaterial = nullptr;
    HeShader* axisShader = nullptr;
};

HeVisualDebugger* vd = nullptr;

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
    helium.InitializeImgui(mWindow);
    gGraphics = helium.GetGraphics();


    gGraphics = helium.GetGraphics();
    gScene = helium.GetScene("Default Scene");

    helium.OnPrepare([&]() {
        pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
        //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
        pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
        //HeCameraManipulatorOrtho manipulator(pCamera);
        pCameraManipulator = gScene->CreateCameraManipulatoFlight("Main Camera Manipulator", pCamera);
        gScene->SetMainCamera(pCamera);

        auto visualDebugger = HeVisualDebugger(helium);
        vd = &visualDebugger;

        {
            auto project = new HeProject("default", "data", "D:\\Workspace\\Reconstruct");

            
        }
        });

    int cnt = 0;

    auto lastTime = HeTime::Now();
    double accTime = 0.0;
    int frameCount = 0;
    int fps = 0;
    int incremental = 0;

    helium.OnFrame([&]() {
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
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //if(cnt == 20)
        //{
        //    auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
        //    //auto pMaterial = dynamic_cast<HeMaterialTextureArray*>(gGraphics->GetMaterial("texture array plane"));
        //    pMaterial->SetIncremental(incremental);
        //    onoff.On(incremental);

        //    incremental++;
        //    if (incremental > capturedFrameCount) {
        //        incremental = 0;
        //    }

        //    cnt = 0;
        //}
        //cnt++;

        gScene->Update((float)delta);
        gScene->Render();

        gGraphics->Flush();

        {
            auto pNode = gScene->GetSceneNodeIMGUI();
            pNode->SetText(format("{}", frameCount));
        }

        gScene->UpdateImgui((float)delta);
        gScene->RenderImgui();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        helium.SetFinished(glfwWindowShouldClose(mWindow));
        });

    helium.Run();

    helium.OnTerminate([&]() {
        helium.TerminateImgui();

        glfwTerminate();
        });

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
