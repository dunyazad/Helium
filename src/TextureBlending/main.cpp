#define USING_FILES_SYSTEM
#include <Helium/Helium.h>
using namespace ArtificialNature;

const int windowWidth = 1024;
const int windowHeight = 1024;

float controlValue = 0.5f;

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

HeThickLines* pDebugGeometry = nullptr;

int capturedFrameCount = 0;
int selectedFrame = -1;
vector<HeFrameInfo*> frameInformations;
vector<float> controlValues(256);
HeProject* project = nullptr;

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

    void On(int index)
    {
        if (-1 < index && index < nodes.size())
        {
            HideAll();

            nodes[index]->SetActive(true);
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

    helium.OnPrepare([&]() {
        cout << "Helium::OnPrepare" << endl;
    });

    //helium.Run();

    helium.InitializeImgui(mWindow);

    gScene = helium.GetScene("Default Scene");

    pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    gScene->SetMainCamera(pCamera);

    for (auto& v : controlValues)
    {
        v = 1.0f;
    }

    {
        auto pNode = gScene->CreateSceneNodeImgui("imgui");
        pNode->SetText("0");
    }

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
        auto pNode = gScene->CreateSceneNode("Debug");

#pragma region [Lines]
        auto pLines = gGraphics->GetGeometryThickLines("Debug");
        pLines->Initialize();
        pLines->SetThickness(1);
        pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pNode->AddGeometry(pLines);

        pDebugGeometry = pLines;

        auto pMaterial = gGraphics->GetMaterial("Debug");

        auto pShader = gGraphics->GetShader("Debug", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
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
    }

    {
        //HeProject project(argv[1], argv[2]);
        project = new HeProject("default", "data", "D:\\Workspace\\Reconstruct");
        capturedFrameCount = project->GetFrames().size();
        vector<float> dataToFragmentShader;


        vector<HeImage*> colorImages;
        for (size_t i = 0; i < capturedFrameCount; i++)
        {
            controlValues[i] = 1.0f;

            auto frame = project->GetFrames()[i];
            frameInformations.push_back(frame);
         
            dataToFragmentShader.push_back(frame->GetCameraInfo()->GetFX());

            auto frameMatrix = frame->GetCameraInfo()->GetTransformMatrix();
            auto framePosition = frame->GetCameraInfo()->GetPosition();
            dataToFragmentShader.push_back(framePosition.x);
            dataToFragmentShader.push_back(framePosition.y);
            dataToFragmentShader.push_back(framePosition.z);

            auto frameDirection = frame->GetCameraInfo()->GetRotation()[2];
            dataToFragmentShader.push_back(frameDirection.x);
            dataToFragmentShader.push_back(frameDirection.y);
            dataToFragmentShader.push_back(frameDirection.z);

            auto frameInverseTransform = glm::value_ptr(frame->GetCameraInfo()->GetInversedTransformMatrix());
            for (int j = 0; j < 16; j++) {
                dataToFragmentShader.push_back(frameInverseTransform[j]);
            }

            auto image = frame->LoadColorImage(gGraphics);
            image->Initialize();
            colorImages.push_back(image);


			auto camera_info = frame->GetCameraInfo();
			auto width = camera_info->GetImageWidth();
			auto height = camera_info->GetImageHeight();
			auto fx = camera_info->GetFX();
			auto hiw = width * 0.5f;
			auto hih = height * 0.5f;

			auto cameraInfo = frame->GetCameraInfo();
			auto frustum = cameraInfo->GetFrustum();
			auto& nr = frustum->GetNormalizedRight();
			auto& nu = frustum->GetNormalizedUp();
			auto& nf = frustum->GetNormalizedFront();
			auto& fp = frustum->GetPosition();

            auto pNode = gScene->CreateSceneNode(format("frame{}", frame->GetFrameIndex()));
            onoff.AddSceneNode(pNode);
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

                auto length = sqrt(fx * fx + hiw * hiw) * 0.01f;
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);

                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));
                pLines->AddColor(glm::vec4(1, 1, 0, 1));

                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightTop() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionRightBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftBottom() * length);
                pLines->AddVertex(fp + frustum->GetDirectionLeftTop() * length);

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
        }

        //for (size_t i = 0; i < dataToFragmentShader.size(); i++)
        //{
        //    printf("%4d : %f\n", i, dataToFragmentShader[i]);
        //}

        HeTextureFloatData* textureFloatData = gGraphics->GetTextureFloatData("float data", dataToFragmentShader);
        textureFloatData->Initialize();

        HeTextureArray* colorTextures = gGraphics->GetTextureArray("color textures", colorImages);
        colorTextures->Initialize();

        auto pNode = gScene->CreateSceneNode("NewMesh");

        auto pGeometry = gGraphics->GetGeometryTriangleSoup("NewMesh");
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
        pGeometry->SetMaterial(pMaterial);
        pMaterial->SetTextureArray(colorTextures);
        pMaterial->SetTextureFloatData(textureFloatData);


        auto pShader = gGraphics->GetShader("blending", "../../res/shader/blending.vs", "../../res/shader/blending.fs");
        //auto pShader = gGraphics->GetShader("reprojection", "../../res/shader/reprojection.vs", "../../res/shader/reprojection.fs");
        pMaterial->SetShader(pShader);

        pShader->Use();
        pShader->SetUniformInt("screenWidth", windowWidth);
        pShader->SetUniformInt("screenHeight", windowHeight);

        pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
        pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

        pShader->SetUniformFloat("controlValue", controlValue);

        pShader->SetUniformInt("frameCount", capturedFrameCount);

        pShader->SetUniformFloatArray("controlValues", controlValues);

        auto mesh = gGraphics->GetGeometryTriangleSoup("Mesh");
        auto nof = mesh->GetFaceCount();
        for (size_t i = 0; i < nof; i++)
        {
            auto vi0 = mesh->GetIndex(i * 3);
            auto vi1 = mesh->GetIndex(i * 3 + 1);
            auto vi2 = mesh->GetIndex(i * 3 + 2);

            auto& v0 = mesh->GetVertex(vi0);
            auto& v1 = mesh->GetVertex(vi1);
            auto& v2 = mesh->GetVertex(vi2);

            auto d10 = glm::normalize(v0 - v1);
            auto d12 = glm::normalize(v2 - v1);
            auto fn = glm::normalize(glm::cross(d10, d12));

            pGeometry->AddTriangle(v0, v1, v2, fn, fn, fn);
        }
    }
    
    

    /*
    {
        auto pNode = gScene->CreateSceneNode("Temp");
        auto pGeometry = gGraphics->GetGeometryTriangleSoup("Temp");
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);


        struct RemappingInfo
        {
            int faceIndex;
            glm::vec3 v0;
            glm::vec3 v1;
            glm::vec3 v2;
            glm::vec3 fn;
            glm::vec3 fc;
            glm::vec3 xAxis;
            glm::vec3 yAxis;
            glm::vec3 zAxis;
        };

        auto mesh = gGraphics->GetGeometry("Mesh");
        auto nof = mesh->GetFaceCount();
        for (size_t i = 0; i < nof; i++)
        {
            auto vi0 = mesh->GetIndex((int)i * 3);
            auto vi1 = mesh->GetIndex((int)i * 3 + 1);
            auto vi2 = mesh->GetIndex((int)i * 3 + 2);

            auto& v0 = mesh->GetVertex(vi0);
            auto& v1 = mesh->GetVertex(vi1);
            auto& v2 = mesh->GetVertex(vi2);

            auto d01 = v1 - v0;
            auto d21 = v1 - v2;
            auto l01 = glm::length2(d01);
            auto l21 = glm::length2(d21);
            d01 = glm::normalize(d01);
            d21 = glm::normalize(d21);
            auto fn = glm::cross(d01, d21);
            auto fc = glm::vec3();
            auto xAxis = glm::vec3();
            auto yAxis = glm::vec3();
            auto zAxis = fn;

            if (l01 > l21)
            {
                fc = v0;
                xAxis = d01;
                yAxis = glm::cross(zAxis, xAxis);
            }
            else
            {
                fc = v2;
                xAxis = d21;
                yAxis = glm::cross(zAxis, xAxis);
            }

            auto tm = glm::mat4();
            tm[0] = glm::vec4(xAxis, 0);
            tm[1] = glm::vec4(yAxis, 0);
            tm[2] = glm::vec4(zAxis, 0);
            tm[3] = glm::vec4(fc, 1);

            auto im = glm::inverse(tm);

            auto angle = glm::angle(glm::vec3(im[0]), glm::vec3(1, 0, 0));
            auto angleAxis = glm::angleAxis(-angle, glm::vec3(0, 0, 1));

            im = glm::mat4(glm::mat3_cast(angleAxis)) * im;

            auto inv0 = im * glm::vec4(v0, 1);
            auto inv1 = im * glm::vec4(v1, 1);
            auto inv2 = im * glm::vec4(v2, 1);

            pGeometry->AddTriangle(glm::vec3(inv0), glm::vec3(inv1), glm::vec3(inv2));
        }
    }
    */


    int cnt = 0;

    auto lastTime = HeTime::Now();
    double accTime = 0.0;
    int frameCount = 0;
    int fps = 0;
    int incremental = 0;
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
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(cnt == 20)
        {
            auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
            //auto pMaterial = dynamic_cast<HeMaterialTextureArray*>(gGraphics->GetMaterial("texture array plane"));
            pMaterial->SetIncremental(incremental);
            onoff.On(incremental);

            incremental++;
            if (incremental > capturedFrameCount) {
                incremental = 0;
            }

            cnt = 0;
        }
        cnt++;

        gScene->Update((float)delta);
        gScene->Render();

        gGraphics->Flush();

 /*       {
            auto pNode = gScene->GetSceneNodeIMGUI();
            pNode->SetText(format("{}", frameCount));
        }*/

        gScene->UpdateImgui((float)delta);
        gScene->RenderImgui();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    helium.TerminateImgui();

    glfwTerminate();
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    pCamera->SetAspectRatio((float)width / (float)height);

    //gGraphics->GetFrameBuffer("FrameBuffer")->Resize(width, height);

    auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
    if (pMaterial)
    {
        auto pShader = pMaterial->GetShader();
        pShader->Use();
        pShader->SetUniformInt("screenWidth", width);
        pShader->SetUniformInt("screenHeight", height);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (pCameraManipulator != nullptr)
    {
        pCameraManipulator->OnKey(window, key, scancode, action, mods);
    }

    if (key == GLFW_KEY_C && action == GLFW_RELEASE)
    {
        //auto image = gGraphics->GetCanvasImage("Capture", mWidth, mHeight);
        //image->CaptureFrame("capture.png");

        vector<string> fileNames;
        for (size_t i = 0; i < 1; i++)
        {
            stringstream ss;
            ss << "texture_" << i << ".png";
            fileNames.push_back(ss.str());
        }

        gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
            auto pNode = dynamic_cast<HeSceneNodeImgui*>(gScene->GetSceneNode("imgui"));
            if (pNode != nullptr) {
                pNode->SetText(format("{}", frameNumber + 1));
            }
            //pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
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
    else if (key == GLFW_KEY_PAGE_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        controlValue += 0.01;

        controlValue = controlValue > 0.0 ? controlValue : 0.0;
        controlValue = controlValue < 1.0 ? controlValue : 1.0;

        cout << "Control Value : " << controlValue << endl;

        auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
        if (pMaterial)
        {
            auto pShader = pMaterial->GetShader();
            pShader->Use();
            pShader->SetUniformFloat("controlValue", controlValue);
        }
    }
    else if (key == GLFW_KEY_PAGE_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        controlValue -= 0.01;

        controlValue = controlValue > 0.0 ? controlValue : 0.0;
        controlValue = controlValue < 1.0 ? controlValue : 1.0;

        cout << "Control Value : " << controlValue << endl;

        auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
        if (pMaterial)
        {
            auto pShader = pMaterial->GetShader();
            pShader->Use();
            pShader->SetUniformFloat("controlValue", controlValue);
        }
    }
    else if (key == GLFW_KEY_KP_ADD && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        {
            controlValues[selectedFrame] += 0.01;
        }
        else
        {
            controlValues[selectedFrame] += 0.001;
        }

        auto pShader = gGraphics->GetShader("blending");
        pShader->Use();
        pShader->SetUniformFloatArray("controlValues", controlValues);

        cout << "controlValues[" << selectedFrame << "] : " << controlValues[selectedFrame] << endl;
    }
    else if (key == GLFW_KEY_KP_SUBTRACT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        {
            controlValues[selectedFrame] -= 0.01;
        }
        else
        {
            controlValues[selectedFrame] -= 0.001;
        }

        auto pShader = gGraphics->GetShader("blending");
        pShader->Use();
        pShader->SetUniformFloatArray("controlValues", controlValues);

        cout << "controlValues[" << selectedFrame << "] : " << controlValues[selectedFrame] << endl;
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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        auto pNode = gScene->GetSceneNode("NewMesh");
        auto pGeometry = pNode->GetGeometry("NewMesh");
        
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        auto faceIndices = pGeometry->RayIntersect(xpos, ypos, gScene->GetMainCamera()->GetProjectionMatrix(), gScene->GetMainCamera()->GetViewMatrix());
        //auto faceIndices = pGeometry->RayIntersect(gScene->GetMainCamera()->GetLocalPosition(), gScene->GetMainCamera()->GetCameraFront());
        auto faceIndex = faceIndices[0];
        auto vi0 = pGeometry->GetIndex(faceIndex * 3);
        auto vi1 = pGeometry->GetIndex(faceIndex * 3 + 1);
        auto vi2 = pGeometry->GetIndex(faceIndex * 3 + 2);

        auto v0 = pGeometry->GetVertex(vi0);
        auto v1 = pGeometry->GetVertex(vi1);
        auto v2 = pGeometry->GetVertex(vi2);
        auto vc = (v0 + v1 + v2) / 3.0f;
        auto vn = pGeometry->GetNormal(vi0);

        float dist = FLT_MAX;
        int nearestFrame = 0;
        for (auto& frame : frameInformations)
        {
            auto camera_info = frame->GetCameraInfo();
            if (camera_info->GetFrustum()->ContainsAll(v0, v1, v2))
            {
                auto& position = camera_info->GetPosition();
                auto distance2 = glm::distance2(vc, position);
                if (distance2 < dist)
                {
                    dist = distance2;
                    nearestFrame = frame->GetFrameIndex();
                }
            }
        }
        selectedFrame = nearestFrame;

        gScene->GetSceneNodeIMGUI()->SetText(format("{}", selectedFrame));

        {
            auto pDebugNode = gScene->GetSceneNode("Debug");
            auto pDebugGeometry = dynamic_cast<HeThickLines*>(pDebugNode->GetGeometry("Debug"));
            pDebugGeometry->AddVertex(v0);
            pDebugGeometry->AddVertex(v1);
            pDebugGeometry->AddVertex(v1);
            pDebugGeometry->AddVertex(v2);
            pDebugGeometry->AddVertex(v2);
            pDebugGeometry->AddVertex(v0);

            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
        }
    }
}

void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (pCameraManipulator != nullptr)
    {
        pCameraManipulator->OnWheel(window, xoffset, yoffset);
    }
}
