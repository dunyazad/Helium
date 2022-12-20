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

HeThickLines* pDebugGeometry = nullptr;

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

HeGeometry* Flatten(const HeProject& project, HeGeometry* from, const string& name, float scale, vector<glm::vec2>& uvs, vector<int>& faceFrameMapping, bool asBoundingBox = false);

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

    helium.InitializeImgui(mWindow);

    gScene = helium.GetScene("Default Scene");

    pCamera = gScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    //pCamera = gScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    gScene->SetMainCamera(pCamera);

    //{
    //    auto pNode = gScene->CreateSceneNodeImgui("imgui");
    //    pNode->SetText("0");
    //}

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
        HeProject project("default", "data", "D:\\Workspace\\Reconstruct");
        vector<float> dataToFragmentShader;

        vector<HeImage*> colorImages;
        for (size_t i = 0; i < project.GetFrames().size(); i++)
        {
            auto frame = project.GetFrames()[i];
         
            dataToFragmentShader.push_back(frame->GetCameraInfo()->GetFX());
            dataToFragmentShader.push_back(frame->GetCameraInfo()->GetFY());
            dataToFragmentShader.push_back(0.0f);
            dataToFragmentShader.push_back(0.0f);

            auto frameMatrix = frame->GetCameraInfo()->GetInversedTransformMatrix();
            for (int j = 0; j < 4; j++) {
                dataToFragmentShader.push_back(frameMatrix[j].x);
                dataToFragmentShader.push_back(frameMatrix[j].y);
                dataToFragmentShader.push_back(frameMatrix[j].z);
                dataToFragmentShader.push_back(frameMatrix[j].w);
            }

            auto image = frame->LoadColorImage(gGraphics);
            image->Initialize();
            colorImages.push_back(image);
        }

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


        auto pShader = gGraphics->GetShader("reprojection", "../../res/shader/reprojection.vs", "../../res/shader/reprojection.fs");
        pMaterial->SetShader(pShader);

        pShader->Use();
        pShader->SetUniformInt("screenWidth", windowWidth);
        pShader->SetUniformInt("screenHeight", windowHeight);

        pShader->SetUniformInt("imageWidth", colorTextures[0].GetWidth());
        pShader->SetUniformInt("imageHeight", colorTextures[0].GetHeight());

        pShader->SetUniformInt("frameCount", project.GetFrames().size());

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

            auto fc = (v0 + v1 + v2) / 3.0f;
            auto fn = mesh->GetNormal(vi0);

            HeFrameInfo* nearestFrame = nullptr;
            float nearestDistance2 = FLT_MAX;

            HeFrameInfo* oppositeFacingFrame = nullptr;
            float dotBetweenCameraAndFaceNormal = FLT_MAX;

            HeFrameInfo* nearUVCenterFrame = nullptr;
            float uvCenterDistance2 = FLT_MAX;
            for (auto& frame : project.GetFrames())
            {
                auto cameraInfo = frame->GetCameraInfo();
                auto cameraFront = glm::vec3(cameraInfo->GetViewMatrix()[3]);
                auto frustum = cameraInfo->GetFrustum();

                if (frustum->ContainsAll(v0, v1, v2))
                {
                    auto ffd = glm::distance2(fc, frame->GetCameraInfo()->GetPosition());
                    if (ffd < nearestDistance2) {
                        nearestDistance2 = ffd;
                        nearestFrame = frame;
                    }

                    auto dot = glm::dot(cameraFront, fn);
                    if (dotBetweenCameraAndFaceNormal > dot) {
                        dotBetweenCameraAndFaceNormal = dot;
                        oppositeFacingFrame = frame;
                    }

                    auto uvc = cameraInfo->WorldToUV(fc);

                    auto dc = glm::distance2(uvc, glm::vec2(0.5f, 0.5f));

                    if (uvCenterDistance2 > dc) {
                        uvCenterDistance2 = dc;
                        nearUVCenterFrame = frame;
                    }
                }
            }

        //if (nearestFrame != nullptr)
        //{
        //    auto cameraInfo = nearestFrame->GetCameraInfo();
        //    auto uv0 = cameraInfo->WorldToUV(v0);
        //    auto uv1 = cameraInfo->WorldToUV(v1);
        //    auto uv2 = cameraInfo->WorldToUV(v2);

        //    auto pGeometry = frameGeometries[nearestFrame->GetFrameIndex()];
        //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
        //}

        //if (oppositeFacingFrame != nullptr)
        //{
        //    auto cameraInfo = oppositeFacingFrame->GetCameraInfo();
        //    auto uv0 = cameraInfo->WorldToUV(v0);
        //    auto uv1 = cameraInfo->WorldToUV(v1);
        //    auto uv2 = cameraInfo->WorldToUV(v2);

        //    auto pGeometry = frameGeometries[oppositeFacingFrame->GetFrameIndex()];
        //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
        //}

            if (nearUVCenterFrame != nullptr)
            {
                auto cameraInfo = nearUVCenterFrame->GetCameraInfo();
                auto uv0 = cameraInfo->WorldToUV(v0);
                auto uv1 = cameraInfo->WorldToUV(v1);
                auto uv2 = cameraInfo->WorldToUV(v2);

                auto frameIndex = nearUVCenterFrame->GetFrameIndex();
                uv0.x += frameIndex;
                uv0.y += frameIndex;

                uv1.x += frameIndex;
                uv1.y += frameIndex;

                uv2.x += frameIndex;
                uv2.y += frameIndex;

                pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
            }
        }
    }
    

    /*
    {
       //HeProject project(argv[1], argv[2]);
       HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

       auto pNode = gScene->CreateSceneNode("NewMesh");
       auto pGeometry = gGraphics->GetGeometryTriangleSoup("NewMesh");
       pNode->AddGeometry(pGeometry);

       auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
       pGeometry->SetMaterial(pMaterial);

       auto pShader = gGraphics->GetShader("reprojection", "../../res/shader/reprojection.vs", "../../res/shader/reprojection.fs");
       pMaterial->SetShader(pShader);


       vector<HeGeometryTriangleSoup*> frameGeometries;
       for (auto& frame : project.GetFrames())
       {
           auto id = format("frame_{}", frame->GetFrameIndex());
           auto pNode = gScene->CreateSceneNode(id);
           auto pGeometry = gGraphics->GetGeometryTriangleSoup(id);
           pGeometry->Initialize();
           frameGeometries.push_back(pGeometry);
           pNode->AddGeometry(pGeometry);

           auto pMaterial = gGraphics->GetMaterialSingleTexture(id);
           pGeometry->SetMaterial(pMaterial);

           auto pShader = gGraphics->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
           pMaterial->SetShader(pShader);

           auto image = frame->LoadColorImage(gGraphics);
           auto pTexture = gGraphics->GetTexture(id, image);
           pTexture->Initialize();
           pMaterial->SetTexture(pTexture);

           //auto cameraInfo = frame->GetCameraInfo();
           //auto cameraFront = glm::vec3(cameraInfo->GetRotation()[2]);

           //pDebugGeometry->AddVertex(cameraInfo->GetPosition());
           //pDebugGeometry->AddVertex(cameraInfo->GetPosition() + cameraFront);

           //pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
           //pDebugGeometry->AddColor(glm::vec4(1, 1, 0, 1));
       }

       auto mesh = gGraphics->GetGeometry("Mesh");
       auto nof = mesh->GetFaceCount();
       for (size_t fi = 0; fi < nof; fi++)
       {
           auto vi0 = mesh->GetIndex((int)fi * 3);
           auto vi1 = mesh->GetIndex((int)fi * 3 + 1);
           auto vi2 = mesh->GetIndex((int)fi * 3 + 2);

           const auto& v0 = mesh->GetVertex(vi0);
           const auto& v1 = mesh->GetVertex(vi1);
           const auto& v2 = mesh->GetVertex(vi2);
           auto fc = (v0 + v1 + v2) / 3.0f;
           auto fn = mesh->GetNormal(vi0);

           HeFrameInfo* nearestFrame = nullptr;
           float nearestDistance2 = FLT_MAX;

           HeFrameInfo* oppositeFacingFrame = nullptr;
           float dotBetweenCameraAndFaceNormal = FLT_MAX;

           HeFrameInfo* nearUVCenterFrame = nullptr;
           float uvCenterDistance2 = FLT_MAX;
           for (auto& frame : project.GetFrames())
           {
               auto cameraInfo = frame->GetCameraInfo();
               auto cameraFront = glm::vec3(cameraInfo->GetViewMatrix()[3]);
               auto frustum = cameraInfo->GetFrustum();

               if (frustum->ContainsAll(v0, v1, v2))
               {
                   auto ffd = glm::distance2(fc, frame->GetCameraInfo()->GetPosition());
                   if (ffd < nearestDistance2) {
                       nearestDistance2 = ffd;
                       nearestFrame = frame;
                   }

                   auto dot = glm::dot(cameraFront, fn);
                   if (dotBetweenCameraAndFaceNormal > dot) {
                       dotBetweenCameraAndFaceNormal = dot;
                       oppositeFacingFrame = frame;
                   }

                   auto uvc = cameraInfo->WorldToUV(fc);

                   auto dc = glm::distance2(uvc, glm::vec2(0.5f, 0.5f));

                   if (uvCenterDistance2 > dc) {
                       uvCenterDistance2 = dc;
                       nearUVCenterFrame = frame;
                   }
               }
           }

           //if (nearestFrame != nullptr)
           //{
           //    auto cameraInfo = nearestFrame->GetCameraInfo();
           //    auto uv0 = cameraInfo->WorldToUV(v0);
           //    auto uv1 = cameraInfo->WorldToUV(v1);
           //    auto uv2 = cameraInfo->WorldToUV(v2);

           //    auto pGeometry = frameGeometries[nearestFrame->GetFrameIndex()];
           //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
           //}

           //if (oppositeFacingFrame != nullptr)
           //{
           //    auto cameraInfo = oppositeFacingFrame->GetCameraInfo();
           //    auto uv0 = cameraInfo->WorldToUV(v0);
           //    auto uv1 = cameraInfo->WorldToUV(v1);
           //    auto uv2 = cameraInfo->WorldToUV(v2);

           //    auto pGeometry = frameGeometries[oppositeFacingFrame->GetFrameIndex()];
           //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
           //}

           if (nearUVCenterFrame != nullptr)
           {
               auto cameraInfo = nearUVCenterFrame->GetCameraInfo();
               auto uv0 = cameraInfo->WorldToUV(v0);
               auto uv1 = cameraInfo->WorldToUV(v1);
               auto uv2 = cameraInfo->WorldToUV(v2);

               auto pGeometry = frameGeometries[nearUVCenterFrame->GetFrameIndex()];
               pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
           }
       }
   }
   */

 /*
    {
        //HeProject project(argv[1], argv[2]);
        HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

        vector<HeGeometryTriangleSoup*> frameGeometries;
        for (auto& frame : project.GetFrames())
        {
            auto id = format("frame_{}", frame->GetFrameIndex());
            auto pNode = gScene->CreateSceneNode(id);
            auto pGeometry = gGraphics->GetGeometryTriangleSoup(id);
            pGeometry->Initialize();
            frameGeometries.push_back(pGeometry);
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterialSingleTexture(id);
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("texture", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
            pMaterial->SetShader(pShader);

            auto image = frame->LoadColorImage(gGraphics);
            auto pTexture = gGraphics->GetTexture(id, image);
            pTexture->Initialize();
            pMaterial->SetTexture(pTexture);







            //auto cameraInfo = frame->GetCameraInfo();
            //auto cameraFront = glm::vec3(cameraInfo->GetRotation()[2]);

            //pDebugGeometry->AddVertex(cameraInfo->GetPosition());
            //pDebugGeometry->AddVertex(cameraInfo->GetPosition() + cameraFront);

            //pDebugGeometry->AddColor(glm::vec4(1, 0, 0, 1));
            //pDebugGeometry->AddColor(glm::vec4(1, 1, 0, 1));
        }

        auto mesh = gGraphics->GetGeometry("Mesh");
        auto nof = mesh->GetFaceCount();
        for (size_t fi = 0; fi < nof; fi++)
        {
            auto vi0 = mesh->GetIndex((int)fi * 3);
            auto vi1 = mesh->GetIndex((int)fi * 3 + 1);
            auto vi2 = mesh->GetIndex((int)fi * 3 + 2);

            const auto& v0 = mesh->GetVertex(vi0);
            const auto& v1 = mesh->GetVertex(vi1);
            const auto& v2 = mesh->GetVertex(vi2);
            auto fc = (v0 + v1 + v2) / 3.0f;
            auto fn = mesh->GetNormal(vi0);

            HeFrameInfo* nearestFrame = nullptr;
            float nearestDistance2 = FLT_MAX;

            HeFrameInfo* oppositeFacingFrame = nullptr;
            float dotBetweenCameraAndFaceNormal = FLT_MAX;

            HeFrameInfo* nearUVCenterFrame = nullptr;
            float uvCenterDistance2 = FLT_MAX;
            for (auto& frame : project.GetFrames())
            {
                auto cameraInfo = frame->GetCameraInfo();
                auto cameraFront = glm::vec3(cameraInfo->GetViewMatrix()[3]);
                auto frustum = cameraInfo->GetFrustum();

                if (frustum->ContainsAll(v0, v1, v2))
                {
                    auto ffd = glm::distance2(fc, frame->GetCameraInfo()->GetPosition());
                    if (ffd < nearestDistance2) {
                        nearestDistance2 = ffd;
                        nearestFrame = frame;
                    }

                    auto dot = glm::dot(cameraFront, fn);
                    if (dotBetweenCameraAndFaceNormal > dot) {
                        dotBetweenCameraAndFaceNormal = dot;
                        oppositeFacingFrame = frame;
                    }

                    auto uvc = cameraInfo->WorldToUV(fc);

                    auto dc = glm::distance2(uvc, glm::vec2(0.5f, 0.5f));

                    if (uvCenterDistance2 > dc) {
                        uvCenterDistance2 = dc;
                        nearUVCenterFrame = frame;
                    }
                }
            }

            //if (nearestFrame != nullptr)
            //{
            //    auto cameraInfo = nearestFrame->GetCameraInfo();
            //    auto uv0 = cameraInfo->WorldToUV(v0);
            //    auto uv1 = cameraInfo->WorldToUV(v1);
            //    auto uv2 = cameraInfo->WorldToUV(v2);

            //    auto pGeometry = frameGeometries[nearestFrame->GetFrameIndex()];
            //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
            //}

            //if (oppositeFacingFrame != nullptr)
            //{
            //    auto cameraInfo = oppositeFacingFrame->GetCameraInfo();
            //    auto uv0 = cameraInfo->WorldToUV(v0);
            //    auto uv1 = cameraInfo->WorldToUV(v1);
            //    auto uv2 = cameraInfo->WorldToUV(v2);

            //    auto pGeometry = frameGeometries[oppositeFacingFrame->GetFrameIndex()];
            //    pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
            //}

            if (nearUVCenterFrame != nullptr)
            {
                auto cameraInfo = nearUVCenterFrame->GetCameraInfo();
                auto uv0 = cameraInfo->WorldToUV(v0);
                auto uv1 = cameraInfo->WorldToUV(v1);
                auto uv2 = cameraInfo->WorldToUV(v2);

                auto pGeometry = frameGeometries[nearUVCenterFrame->GetFrameIndex()];
                pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
            }
        }
    }
*/

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

    /*
    {
        vector<HeImage*> images;
        for (size_t i = 0; i < 256; i++)
        {
            auto image = gGraphics->GetImage(format("texture_{}.png", i), format("D:\\Resources\\2D\\ImageScequence\\texture_{}.png", i));
            image->Initialize();
            images.push_back(image);
        }
        
        auto pTextureArray = gGraphics->GetTextureArray("texture array", images);
        pTextureArray->Initialize();

        auto pNode = gScene->CreateSceneNode("texture array plane");
        auto pGeometry = gGraphics->GetGeometryPlane("texture array plane", 1, 1, 10, 10, HePlaneType::XY);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterialTextureArray("texture array plane");
        
        auto pShader = gGraphics->GetShader("textureArray", "../../res/shader/textureArray.vs", "../../res/shader/textureArray.fs");
        pMaterial->SetShader(pShader);

        pMaterial->SetTextureArray(pTextureArray);

        pGeometry->SetMaterial(pMaterial);
    }
    */


    int textureIndex = 0;

    int cnt = 0;


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
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(cnt == 300)
        {
            auto pMaterial = gGraphics->GetMaterialReprojection("reprojection");
            //auto pMaterial = dynamic_cast<HeMaterialTextureArray*>(gGraphics->GetMaterial("texture array plane"));
            pMaterial->SetTextureIndex(textureIndex);

            textureIndex++;
            if (textureIndex > 512) {
                textureIndex = 0;
            }

            cnt = 0;
        }
        cnt++;

        gScene->Update((float)delta);
        gScene->Render();

        gScene->UpdateImgui((float)delta);
        gScene->RenderImgui();

        gGraphics->Flush();

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

HeGeometry* Flatten(const HeProject& project, HeGeometry* from, const string& name, float scale, vector<glm::vec2>& uvs, vector<int>& faceFrameMapping, bool asBoundingBox)
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
    faceFrameMapping.resize(flatteningInfos.size() * 3);

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

            auto translatedUV0 = info.normalizedUV0;
            auto translatedUV1 = info.normalizedUV1;
            auto translatedUV2 = info.normalizedUV2;

            translatedUV0.x += offsetX;
            translatedUV0.y += offsetY;
            translatedUV1.x += offsetX;
            translatedUV1.y += offsetY;
            translatedUV2.x += offsetX;
            translatedUV2.y += offsetY;

            if (translatedUV0.x < 0 || translatedUV0.y < 0) {
                cout << "Error. translatedUV0: " << translatedUV0 << endl;
            }
            if (translatedUV1.x < 0 || translatedUV1.y < 0) {
                cout << "Error. translatedUV1: " << translatedUV1 << endl;
            }
            if (translatedUV2.x < 0 || translatedUV2.y < 0) {
                cout << "Error. translatedUV2: " << translatedUV2 << endl;
            }

            auto fi = info.faceIndex;
            auto uvi0 = fi * 3;
            auto uvi1 = fi * 3 + 1;
            auto uvi2 = fi * 3 + 2;
            uvs[uvi0].x = translatedUV0.x;
            uvs[uvi0].y = translatedUV0.y;
            uvs[uvi1].x = translatedUV1.x;
            uvs[uvi1].y = translatedUV1.y;
            uvs[uvi2].x = translatedUV2.x;
            uvs[uvi2].y = translatedUV2.y;

            faceFrameMapping[fi] = nearestFrame->GetFrameIndex();

            if (asBoundingBox == false)
            {
                uvMesh->AddTriangle(translatedUV0, translatedUV1, translatedUV2);

                uvMesh->AddUV(info.normalizedUV0);
                uvMesh->AddUV(info.normalizedUV1);
                uvMesh->AddUV(info.normalizedUV2);
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
                tempAABB.Extend(translatedUV0);
                tempAABB.Extend(translatedUV1);
                tempAABB.Extend(translatedUV2);

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

                HeAABB uvAABB;
                uvAABB.Extend(info.normalizedUV0);
                uvAABB.Extend(info.normalizedUV1);
                uvAABB.Extend(info.normalizedUV2);

                auto uvmin = uvAABB.GetMin();
                auto uvmax = uvAABB.GetMax();

                uvMesh->AddUV(glm::vec2(uvmin.x, uvmin.y));
                uvMesh->AddUV(glm::vec2(uvmin.x, uvmax.y));
                uvMesh->AddUV(glm::vec2(uvmax.x, uvmax.y));

                uvMesh->AddUV(glm::vec2(uvmin.x, uvmin.y));
                uvMesh->AddUV(glm::vec2(uvmax.x, uvmax.y));
                uvMesh->AddUV(glm::vec2(uvmax.x, uvmin.y));
            }
        }
        else
        {
            cout << "Error. nearest frame is null" << endl;
        }
    }

    return uvMesh;
}
