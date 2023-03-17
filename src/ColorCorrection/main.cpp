#define USING_FILES_SYSTEM
#include <Helium/Helium.h>
#include <omp.h>
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
HeCameraManipulatorBase* pCameraManipulator = nullptr;

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
        pCameraManipulator = gScene->CreateCameraManipulatoObital("Main Camera Manipulator", pCamera);
        gScene->SetMainCamera(pCamera);

        vd = gScene->GetVisualDebugger();
        //onoff = vd->GetOnOff();

        auto project = new HeProject("default", "data", "D:/Resources/Scan");
        auto pMesh = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:/Resources/Scan/projects/default/data/reconstructed/04_Fixed.stl");
        
        {
            vector<HeRay> rays;
            auto frame = project->GetFrames()[0];
            auto camera_info = frame->GetCameraInfo();
            auto frustum = camera_info->GetFrustum();
            auto width = camera_info->GetColorImageWidth();
            auto height = camera_info->GetColorImageHeight();
            auto position = camera_info->GetPosition();
            for (int h = 0; h < height; h++)
            {
                for (int w = 0; w < width; w++)
                {
                    auto p = camera_info->UVToWorld(glm::vec2(w / width, h / height));
                    auto d = glm::normalize(p - position);
                    rays.push_back(HeRay(position, d));
                    //auto wp = pMesh->RayIntersect(position, d);
                    //if (wp.size() > 0) {
                    //    auto& v = pMesh->GetVertex(wp[0]);
                    //    vd->AddBox(v, 0.01f, 0.01f, 0.01f, HeColor::RED);
                    //}
                }
            }

            auto faceCount = pMesh->GetFaceCount();
            for (size_t i = 0; i < faceCount; i++)
            {
                auto i0 = pMesh->GetIndex(i * 3);
                auto i1 = pMesh->GetIndex(i * 3 + 1);
                auto i2 = pMesh->GetIndex(i * 3 + 2);
                auto& v0 = pMesh->GetVertex(i0);
                auto& v1 = pMesh->GetVertex(i1);
                auto& v2 = pMesh->GetVertex(i2);

                if (frustum->ContainsAny(v0, v1, v2))
                {
                    vd->AddTriangle(v0, v1, v2);
                }
            }

            auto wlu = camera_info->UVToWorld(glm::vec2(0, 1));
            auto wld = camera_info->UVToWorld(glm::vec2(0, 0));
            auto wru = camera_info->UVToWorld(glm::vec2(1, 1));
            auto wrd = camera_info->UVToWorld(glm::vec2(1, 0));
            auto wic = camera_info->UVToWorld(glm::vec2(0.5f, 0.5f));
            vd->AddBox(wlu * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::RED);
            vd->AddBox(wld * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::RED);
            vd->AddBox(wru * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::RED);
            vd->AddBox(wrd * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::RED);
            vd->AddBox(wic * 0.001f, 0.01f, 0.01f, 0.01f);

            auto whlu = camera_info->UVToWorld(glm::vec2(0.25f, 0.75f));
            auto whld = camera_info->UVToWorld(glm::vec2(0.25f, 0.25f));
            auto whru = camera_info->UVToWorld(glm::vec2(0.75f, 0.75f));
            auto whrd = camera_info->UVToWorld(glm::vec2(0.75f, 0.25f));
            vd->AddBox(whlu * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::BLUE);
            vd->AddBox(whld * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::BLUE);
            vd->AddBox(whru * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::BLUE);
            vd->AddBox(whrd * 0.001f, 0.01f, 0.01f, 0.01f, HeColor::BLUE);

            vd->AddLine(camera_info->GetPosition(), wlu * 0.001f, HeColor::RED, HeColor::RED);
            vd->AddLine(camera_info->GetPosition(), wld * 0.001f, HeColor::RED, HeColor::RED);
            vd->AddLine(camera_info->GetPosition(), wru * 0.001f, HeColor::RED, HeColor::RED);
            vd->AddLine(camera_info->GetPosition(), wrd * 0.001f, HeColor::RED, HeColor::RED);

            vd->AddLine(frustum->GetPosition() + glm::vec3(0.1, 0.1, 0.1f), frustum->GetLeftPlaneNormal() * 0.01f, HeColor::RED, HeColor::WHITE);
            vd->AddLine(frustum->GetPosition() + glm::vec3(0.1, 0.1, 0.1f), frustum->GetRightPlaneNormal() * 0.01f, HeColor::GREEN, HeColor::BLACK);
            vd->AddLine(frustum->GetPosition() + glm::vec3(0.1, 0.1, 0.1f), frustum->GetUpperPlaneNormal() * 0.01f, HeColor::BLUE, HeColor::YELLOW);
            vd->AddLine(frustum->GetPosition() + glm::vec3(0.1, 0.1, 0.1f), frustum->GetLowerPlaneNormal() * 0.01f, HeColor::CYAN, HeColor::CYAN);

   /*         auto time = HeTime("parallel");
            #pragma omp parallel for
            for (size_t i = 0; i < rays.size(); i++)
            {
                auto ray = rays[i];
                auto wp = pMesh->RayIntersect(ray.GetOrigin(), ray.GetDirection());
            }
            time.Stop();*/
        }


        {
            float scale = 0.001;

            int frameNumber = 0;
            for (auto& frame : project->GetFrames())
            {
                //auto frameA = project->GetFrames()[0];
                auto frameA = frame;
                auto image_A = frameA->LoadColorImage(gGraphics);
                auto texture_A = gGraphics->GetTexture(image_A->GetName(), image_A);
                texture_A->Initialize();
                auto camera_info_A = frameA->GetCameraInfo();
                auto frustum_A = camera_info_A->GetFrustum();
                auto transform_A = frustum_A->GetTransform();
                auto im = glm::inverse(transform_A);
                auto p_A = frustum_A->GetPosition();
                auto ic_A = frustum_A->GetImageCenter();
                auto lu_A = frustum_A->GetImageLeftUp();
                auto ld_A = frustum_A->GetImageLeftDown();
                auto ru_A = frustum_A->GetImageRightUp();
                auto rd_A = frustum_A->GetImageRightDown();

                auto ip_A = (im * glm::vec4(frustum_A->GetPosition(), 1));
                auto iic_A = (im * glm::vec4(frustum_A->GetImageCenter(), 1));
                auto ilu_A = (im * glm::vec4(frustum_A->GetImageLeftUp(), 1));
                auto ild_A = (im * glm::vec4(frustum_A->GetImageLeftDown(), 1));
                auto iru_A = (im * glm::vec4(frustum_A->GetImageRightUp(), 1));
                auto ird_A = (im * glm::vec4(frustum_A->GetImageRightDown(), 1));

                //ip_A.z = 0;
                //iic_A.z = 0;
                //ilu_A.z = 0;
                //ild_A.z = 0;
                //iru_A.z = 0;
                //ird_A.z = 0;

                //vd->AddLine(p_A, lu_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(p_A, ld_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(p_A, ru_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(p_A, rd_A * scale, HeColor::RED, HeColor::RED);

                //vd->AddLine(ic_A * scale, lu_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(ic_A * scale, ld_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(ic_A * scale, ru_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(ic_A * scale, rd_A * scale, HeColor::RED, HeColor::RED);

                //vd->AddLine(lu_A * scale, ru_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(ru_A * scale, rd_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(rd_A * scale, ld_A * scale, HeColor::RED, HeColor::RED);
                //vd->AddLine(ld_A * scale, lu_A * scale, HeColor::RED, HeColor::RED);

                vd->AddPlane(lu_A * scale, ld_A * scale, ru_A * scale, rd_A * scale, texture_A);

                //auto frameB = project->GetFrames()[2];
                //auto image_B = frameB->LoadColorImage(gGraphics, "frame 2");
                //auto texture_B = gGraphics->GetTexture(image_B->GetName(), image_B);
                //texture_B->Initialize();
                //auto camera_info_B = frameB->GetCameraInfo();
                //auto frustum_B = camera_info_B->GetFrustum();
                //auto p_B = frustum_B->GetPosition();
                //auto ic_B = frustum_B->GetImageCenter();
                //auto lu_B = frustum_B->GetImageLeftUp();
                //auto ld_B = frustum_B->GetImageLeftDown();
                //auto ru_B = frustum_B->GetImageRightUp();
                //auto rd_B = frustum_B->GetImageRightDown();

                //auto ip_B = (im * glm::vec4(frustum_B->GetPosition(), 1));
                //auto iic_B = (im * glm::vec4(frustum_B->GetImageCenter(), 1));
                //auto ilu_B = (im * glm::vec4(frustum_B->GetImageLeftUp(), 1));
                //auto ild_B = (im * glm::vec4(frustum_B->GetImageLeftDown(), 1));
                //auto iru_B = (im * glm::vec4(frustum_B->GetImageRightUp(), 1));
                //auto ird_B = (im * glm::vec4(frustum_B->GetImageRightDown(), 1));

                //ip_B.z = 0;
                //iic_B.z = 0;
                //ilu_B.z = 0;
                //ild_B.z = 0;
                //iru_B.z = 0;
                //ird_B.z = 0;

                ////vd->AddLine(ilu_B * scale, iru_B * scale, HeColor::BLUE, HeColor::BLUE);
                ////vd->AddLine(iru_B * scale, ird_B * scale, HeColor::BLUE, HeColor::BLUE);
                ////vd->AddLine(ird_B * scale, ild_B * scale, HeColor::BLUE, HeColor::BLUE);
                ////vd->AddLine(ild_B * scale, ilu_B * scale, HeColor::BLUE, HeColor::BLUE);

                //vd->AddPlane(lu_B * scale, ld_B * scale, ru_B * scale, rd_B * scale, texture_B);
            }
        }
        

        //{
        //    auto pNode = gScene->CreateSceneNode("Mesh");
        //    auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:/Resources/Scan/projects/default/data/reconstructed/04_Fixed.stl");

        //    //pGeometry->SetFillMode(HeGeometry::Wireframe);
        //    pGeometry->Initialize();
        //    pNode->AddGeometry(pGeometry);

        //    auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        //    pGeometry->SetMaterial(pMaterial);

        //    auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        //    pMaterial->SetShader(pShader);
        //}
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

        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        //glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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
        //onoff->Previous();
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        //onoff->Next();
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
