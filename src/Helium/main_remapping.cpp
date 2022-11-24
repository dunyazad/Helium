#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Helium.h"

using namespace ArtificialNature;

#include <NanoVG/nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <NanoVG/nanovg_gl.h>

//#pragma comment(lib, "nanovg.lib")

const int mWidth = 1024;
const int mHeight = 1024;
//const int mWidth = 512;
//const int mHeight = 512;




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);

HeGraphics* gGraphics = nullptr;
HePerspectiveCamera* pCameraPerspective = nullptr;
HeOrthogonalCamera* pCameraOrtho = nullptr;
HeCamera* pCamera = nullptr;
HeCameraManipulatorFlight* pCameraManipulatorFlight = nullptr;
HeCameraManipulatorOrtho* pCameraManipulatorOrtho = nullptr;
HeCameraManipulatorBase* pCameraManipulator = nullptr;

HeOctreeGeometry* pOctree = nullptr;

class FrameDebugger
{
public:
    FrameDebugger(HeGraphics* gGraphics, HeScene* scene, const HeProject& project) : graphics(graphics), scene(scene), project(project) {}

    void Initialize()
    {
        const auto& frames = project.GetFrames();
        for (auto& frame : frames)
        {
            auto cameraInfo = frame->GetCameraInfo();
            auto frustum = cameraInfo->GetFrustum();
            
            auto width = cameraInfo->GetImageWidth();
            auto height = cameraInfo->GetImageHeight();
            auto fx = cameraInfo->GetFX();
            auto hiw = width * 0.5f;
            auto hih = height * 0.5f;

            auto& nr = frustum->GetNormalizedRight();
            auto& nu = frustum->GetNormalizedUp();
            auto& nf = frustum->GetNormalizedFront();
            auto& fp = frustum->GetPosition();

            auto pNode = scene->CreateSceneNode(format("frame{}", frame->GetFrameIndex()));
            frameSceneNodes.push_back(pNode);
            {
                auto pLines = gGraphics->GetGeometryThickLines(format("frame{}_Lines", frame->GetFrameIndex()));
                pLines->Initialize();
                pLines->SetThickness(1);
                pLines->SetDrawingMode(HeGeometry::DrawingMode::Lines);
                pNode->AddGeometry(pLines);

                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nr * 0.01f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nu * 0.01f);
                pLines->AddVertex(fp);
                pLines->AddVertex(fp + nf * 0.01f);

                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(1, 0, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 1, 0, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));
                pLines->AddColor(glm::vec4(0, 0, 1, 1));

                auto length = sqrt(fx * fx + hiw * hiw) * 0.001f;
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
    }

    void ShowAll() {
        for (auto& node : frameSceneNodes)
        {
            node->SetActive(true);
        }
    }

    void HideAll() {
        for (auto& node : frameSceneNodes)
        {
            node->SetActive(true);
        }
    }

    void ShowOne(int frameIndex) {
        if (frameIndex < frameSceneNodes.size()) {
            frameSceneNodes[frameIndex]->SetActive(true);
        }
    }

    void ShowSome(const vector<int>& frameIndices) {
        for (auto& frameIndex : frameIndices)
        {
            if (frameIndex < frameSceneNodes.size()) {
                frameSceneNodes[frameIndex]->SetActive(true);
            }
        }
    }

protected:
    HeGraphics* graphics;
    HeScene* scene;
    const HeProject& project;
    vector<HeSceneNode*> frameSceneNodes;
};

class FrameSelector
{
public:
    FrameSelector(HeGraphics* gGraphics, HeScene* scene, const HeProject& project, HeGeometry* reconstructedMesh)
        : graphics(graphics), scene(scene), project(project), reconstructedMesh(reconstructedMesh)
    {
        pLinesSceneNode = scene->CreateSceneNode("Picking Debugging Lines");

        pLinesGeometry = gGraphics->GetGeometryThickLines("Gizmo");
        pLinesGeometry->Initialize();
        pLinesGeometry->SetThickness(1);
        pLinesGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pLinesSceneNode->AddGeometry(pLinesGeometry);

        //pLines->AddVertex(glm::vec3(-1024, 0, 0));
        //pLines->AddVertex(glm::vec3(1024, 0, 0));
        //pLines->AddVertex(glm::vec3(0, -1024, 0));
        //pLines->AddVertex(glm::vec3(0, 1024, 0));
        //pLines->AddVertex(glm::vec3(0, 0, -1024));
        //pLines->AddVertex(glm::vec3(0, 0, 1024));

        //pLines->AddColor(glm::vec4(1, 0, 0, 1));
        //pLines->AddColor(glm::vec4(1, 0, 0, 1));
        //pLines->AddColor(glm::vec4(0, 1, 0, 1));
        //pLines->AddColor(glm::vec4(0, 1, 0, 1));
        //pLines->AddColor(glm::vec4(0, 0, 1, 1));
        //pLines->AddColor(glm::vec4(0, 0, 1, 1));

        auto pMaterial = gGraphics->GetMaterial("Gizmo Materials");

        auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLinesGeometry->SetMaterial(pMaterial);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }

    void PickTriangle(float screenX, float screenY)
    {
        auto faceIndices = reconstructedMesh->RayIntersect(screenX, screenY, pCamera->GetProjectionMatrix(), pCamera->GetViewMatrix());
        if(faceIndices.size() != 0)
        //for (auto& fi : faceIndices)
        {
            auto fi = faceIndices[0];
            auto vi0 = reconstructedMesh->GetIndex(fi * 3);
            auto vi1 = reconstructedMesh->GetIndex(fi * 3 + 1);
            auto vi2 = reconstructedMesh->GetIndex(fi * 3 + 2);
            auto& v0 = reconstructedMesh->GetVertex(vi0);
            auto& v1 = reconstructedMesh->GetVertex(vi1);
            auto& v2 = reconstructedMesh->GetVertex(vi2);

            pLinesGeometry->AddVertex(v0);
            pLinesGeometry->AddVertex(v1);
            pLinesGeometry->AddVertex(v1);
            pLinesGeometry->AddVertex(v2);
            pLinesGeometry->AddVertex(v2);
            pLinesGeometry->AddVertex(v0);

            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
            pLinesGeometry->AddColor(glm::vec4(1, 1, 0, 1));
        }
    }

protected:
    HeGraphics* graphics;
    HeScene* scene;
    const HeProject& project;
    vector<HeSceneNode*> frameSceneNodes;
    HeGeometry* reconstructedMesh;

    HeSceneNode* pLinesSceneNode;
    HeThickLines* pLinesGeometry;
};

class OctreeDebugger
{
public:
    OctreeDebugger(HeGraphics* gGraphics, HeScene* scene, HeGeometry* geometry)
        : graphics(graphics), scene(scene), geometry(geometry)
    {
        stack<const HeOctreeGeometry*> os;
        os.push(geometry->GetOctree());

        vector<vector<const HeAABB*>> aabbs;
        while (os.empty() == false)
        {
            auto currentOctree = os.top();
            os.pop();

            auto depth = currentOctree->GetDepth();
            if (aabbs.size() < depth + 1) {
                aabbs.resize(depth + 1);
            }
            aabbs[depth].push_back(currentOctree->GetAABB());

            for (int i = 0; i < 8; i++)
            {
                if (currentOctree->GetSubvolumes()[i] != nullptr) {
                    os.push(currentOctree->GetSubvolumes()[i]);
                }
            }
        }

        for (size_t depth = 0; depth < aabbs.size(); depth++)
        {
            auto pNode = scene->CreateSceneNode(format("AABB Debugging Lines {}", depth));
            aabbSceneNodes.push_back(pNode);

            auto pGeometry = gGraphics->GetGeometryThickLines(format("AABB Debugging Lines {}", depth));
            pGeometry->Initialize();
            pGeometry->SetThickness(1);
            pGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);
            pNode->AddGeometry(pGeometry);

            auto pMaterial = gGraphics->GetMaterial("thick lines");

            auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
            pMaterial->SetShader(pShader);

            pGeometry->SetMaterial(pMaterial);

            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            int count = 0;
            for (auto aabb : aabbs[depth])
            {
                auto& vmin = aabb->GetMin();
                auto& vmax = aabb->GetMax();

                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));

                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));

                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));
                pGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));

                
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));

                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));

                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));
                pGeometry->AddColor(glm::vec4(0, 1, 0, 0.2f));

                count++;
            }
        }

        pLinesSceneNode = scene->CreateSceneNode("Picking Debugging Lines");

        pLinesGeometry = gGraphics->GetGeometryThickLines("Gizmo");
        pLinesGeometry->Initialize();
        pLinesGeometry->SetThickness(1);
        pLinesGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);
        pLinesSceneNode->AddGeometry(pLinesGeometry);

        auto pMaterial = gGraphics->GetMaterial("thick lines");

        auto pShader = gGraphics->GetShader("thick lines", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        pMaterial->SetShader(pShader);

        pLinesGeometry->SetMaterial(pMaterial);


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }

    void HideAll() {
        for (auto& pNode : aabbSceneNodes)
        {
            pNode->SetActive(false);
        }
    }

    void Previous()
    {
        HideAll();

        if (currentDepth - 1 > 0) {
            aabbSceneNodes[currentDepth - 1]->SetActive(true);
            currentDepth--;
        }
    }

    void Next()
    {
        HideAll();

        if (currentDepth + 1 < aabbSceneNodes.size()) {
            aabbSceneNodes[currentDepth + 1]->SetActive(true);
            currentDepth++;
        }
    }

    void Pick(float screenX, float screenY, HeCamera* pCamera)
    {
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        float winX = (float)screenX;
        float winY = (float)viewport[3] - (float)screenY;

        auto u = winX / viewport[2] - 0.5f;
        auto v = winY / viewport[3] - 0.5f;

        const auto& projectionMatrix = pCamera->GetProjectionMatrix();
        const auto& viewMatrix = pCamera->GetViewMatrix();

        auto pp = glm::unProject(glm::vec3(winX, winY, 1), glm::identity<glm::mat4>(), projectionMatrix * viewMatrix, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

        auto rayOrigin = glm::vec3(glm::inverse(viewMatrix)[3]);
        auto rayDirection = glm::normalize(pp - rayOrigin);

        pLinesGeometry->AddVertex(rayOrigin);
        pLinesGeometry->AddVertex(rayOrigin + rayDirection * 1024.0f);
        pLinesGeometry->AddColor(glm::vec4(1, 0, 0, 1));
        pLinesGeometry->AddColor(glm::vec4(1, 0, 0, 1));

        vector<HeOctreeGeometry*> rio;
        const auto octree = const_cast<HeOctreeGeometry*>(geometry->GetOctree());
        if (octree->GetRayInersectingOcrees(rayOrigin, rayDirection, rio))
        {
            for (auto& o : rio)
            {
                auto& vmin = o->GetAABB()->GetMin();
                auto& vmax = o->GetAABB()->GetMax();

                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));

                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmin.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));

                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmin.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmax.x, vmax.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmax.y, vmax.z));
                //pLinesGeometry->AddVertex(glm::vec3(vmin.x, vmin.y, vmax.z));


                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));

                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));

                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));
                //pLinesGeometry->AddColor(glm::vec4(0, 0, 1, 0.2f));

                for (auto& fi : o->GetContainingFaceIndices())
                {
                    auto geometry = o->GetGeometry();
                    auto vi0 = geometry->GetIndex(fi * 3);
                    auto vi1 = geometry->GetIndex(fi * 3 + 1);
                    auto vi2 = geometry->GetIndex(fi * 3 + 2);
                    const auto& v0 = geometry->GetVertex(vi0);
                    const auto& v1 = geometry->GetVertex(vi1);
                    const auto& v2 = geometry->GetVertex(vi2);
                    
                    glm::vec2 baryPosition;
                    float distance = 0;
                    if (glm::intersectRayTriangle(rayOrigin, rayDirection, v0, v1, v2, baryPosition, distance))
                    {
                        if (distance > 0) {
                            pLinesGeometry->AddVertex(v0);
                            pLinesGeometry->AddVertex(v1);
                            pLinesGeometry->AddVertex(v1);
                            pLinesGeometry->AddVertex(v2);
                            pLinesGeometry->AddVertex(v2);
                            pLinesGeometry->AddVertex(v0);

                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                            pLinesGeometry->AddColor(glm::vec4(1, 1, 1, 1));
                        }
                    }
                }
            }
        }
    }

protected:
    HeGraphics* graphics = nullptr;
    HeScene* scene = nullptr;
    HeGeometry* geometry = nullptr;
    vector<HeSceneNode*> aabbSceneNodes;

    HeSceneNode* pLinesSceneNode = nullptr;
    HeThickLines* pLinesGeometry = nullptr;

    int currentDepth = 0;
};

FrameSelector* pFrameSelector = nullptr;

OctreeDebugger* od = nullptr;

int main(int argc, char* argv[])
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
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

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




    Helium helium("helium", mWidth, mHeight);
    gGraphics = helium.GetGraphics();

    auto pScene = helium.GetScene("Default Scene");

    {
        pCameraPerspective = pScene->CreatePerspectiveCamera("Main Camera", 0, 0, mWidth, mHeight);
        pCamera = pCameraPerspective;
        //HeCameraManipulatorFlight manipulator(pCameraPerspective);
        pCameraPerspective->SetLocalPosition(glm::vec3(0.5f, 0.5f, 0.0f));
        pCameraManipulatorFlight = new HeCameraManipulatorFlight(pCameraPerspective);
        pCameraManipulator = pCameraManipulatorFlight;
        pScene->SetMainCamera(pCameraPerspective);
        pCameraManipulatorFlight->ApplyManipulation();
    }
    //{
    //    pCameraOrtho = pScene->CreateOrthogonalCamera("Main Camera", 0, 0, mWidth, mHeight);
    //    pCamera = pCameraOrtho;
    //    HeCameraManipulatorOrtho manipulator(pCameraOrtho);
    //    pCameraManipulatorOrtho = &manipulator;
    //    pCameraManipulator = pCameraManipulatorOrtho;
    //    pScene->SetMainCamera(pCameraOrtho);
    //}

    {
        auto pNode = pScene->CreateSceneNode("Gizmo Node");
        
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
        auto pNode = pScene->CreateSceneNode("Mesh");
        auto pGeometry = HeResourceIO::ReadSTLFile(gGraphics, "Mesh", "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\04_Fixed.stl");
        //pGeometry->SetFillMode(HeGeometry::Wireframe);
        pGeometry->Initialize();
        pNode->AddGeometry(pGeometry);

        auto pMaterial = gGraphics->GetMaterial("Mesh Material");
        pGeometry->SetMaterial(pMaterial);

        auto pShader = gGraphics->GetShader("vertex", "../../res/shader/vertex.vs", "../../res/shader/vertex.fs");
        pMaterial->SetShader(pShader);

        //HeResourceIO::WriteOBJFile(gGraphics, pGeometry->GetName(), "D:\\Workspace\\Reconstruct\\projects\\default\\data\\reconstructed\\TestOBJ.obj");

        od = new OctreeDebugger(gGraphics, pScene, pGeometry);
    }

    HeProject project("default", "data", "D:\\Workspace\\Reconstruct");

    FrameDebugger frameDebugger(gGraphics, pScene, project);
    frameDebugger.Initialize();

    FrameSelector frameSelector(gGraphics, pScene, project, gGraphics->GetGeometry("Mesh"));
    pFrameSelector = &frameSelector;

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

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //pScene->GetSceneNode("Plane")->SetActive(true);

        pScene->Update((float)delta);
        pScene->Render();

        gGraphics->Flush();
        

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

 /*       vector<string> fileNames;
        for (size_t i = 0; i < 37; i++)
        {
            stringstream ss;
            ss << "CaptureFileName_" << i << ".png";
            fileNames.push_back(ss.str());
        }

        gGraphics->SerialFrameCapture(fileNames, [&](int frameNumber) {
            pCamera->SetLocalPosition(pCamera->GetLocalPosition() + glm::vec3(0.0f, 1.0f, 0.0f));
        });*/
    }
    else if (key == GLFW_KEY_LEFT_BRACKET && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (od != nullptr)
        {
            od->Previous();
        }
    }
    else if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        if (od != nullptr)
        {
            od->Next();
        }
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

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if (pCameraManipulator != nullptr)
        {
            auto st = HeTime::Now();
            pFrameSelector->PickTriangle((float)pCameraManipulator->GetLastMousePositionX(), (float)pCameraManipulator->GetLastMousePositionY());
            cout << "Naive: " << HeTime::DeltaMili(st) << " miliseconds" << endl;

            st = HeTime::Now();
            od->Pick((float)pCameraManipulator->GetLastMousePositionX(), (float)pCameraManipulator->GetLastMousePositionY(), pCamera);
            cout << "Octree: " << HeTime::DeltaMili(st) << " miliseconds" << endl;
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
