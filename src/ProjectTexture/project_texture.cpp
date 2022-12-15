#include <Helium/Helium.h>
using namespace ArtificialNature;

const int windowWidth = 1024;
const int windowHeight = 1024;

HeGraphics* gGraphics = nullptr;
HePerspectiveCamera* pCamera = nullptr;
HeCameraManipulatorFlight* pCameraManipulator = nullptr;

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

    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glEnable(GL_MULTISAMPLE);




    Helium helium("helium", windowWidth, windowHeight);
    gGraphics = helium.GetGraphics();

    auto pScene = helium.GetScene("Default Scene");

    pCamera = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    //pCamera = pScene->CreateOrthogonalCamera("Main Camera", 0, 0, windowWidth, windowHeight);
    HeCameraManipulatorFlight manipulator(pCamera);
    pCamera->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
    //HeCameraManipulatorOrtho manipulator(pCamera);
    pCameraManipulator = &manipulator;
    pScene->SetMainCamera(pCamera);

    {
        auto pNode = pScene->CreateSceneNode("Mesh");
        auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", argv[3]);

        if (pGeometry == nullptr) {
            cout << "pGeometry == nullptr" << endl;
        }

        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        //pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "./res/shader/vertex.vs", "./res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");
    }

    {
        HeProject project(argv[1], argv[2]);

        cout << "number of frames " << project.GetFrames().size() << endl;

        vector<HeGeometryTriangleSoup*> frameGeometries;
        for (auto& frame : project.GetFrames())
        {
            auto id = format("frame_{}", frame->GetFrameIndex());
            auto pNode = pScene->CreateSceneNode(id);
            auto pGeometry = gGraphics->GetGeometryTriangleSoup(id);
            pGeometry->Initialize();
            frameGeometries.push_back(pGeometry);
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterialSingleTexture(id);
            pGeometry->SetMaterial(pMaterial);

            auto pShader = gGraphics->GetShader("texture", "./res/shader/texture.vs", "./res/shader/texture.fs");
            pMaterial->SetShader(pShader);

            auto image = frame->LoadColorImage(gGraphics);
            auto pTexture = gGraphics->GetTexture(id, image);
            pTexture->Initialize();
            pMaterial->SetTexture(pTexture);
        }

        auto mesh = gGraphics->GetGeometry("Mesh");
        auto nof = mesh->GetFaceCount();
        for (size_t fi = 0; fi < nof; fi++)
        {
            auto vi0 = mesh->GetIndex(fi * 3);
            auto vi1 = mesh->GetIndex(fi * 3 + 1);
            auto vi2 = mesh->GetIndex(fi * 3 + 2);

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

        HeResourceIO::WriteOBJFile(gGraphics, frameGeometries, argv[4]);
    }

    system("break");

    return 0;
}