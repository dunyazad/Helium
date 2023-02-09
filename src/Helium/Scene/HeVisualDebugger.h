#pragma once

#include <Helium/Core/HeliumCommon.h>
#include <Helium/Core/HeColor.h>

namespace ArtificialNature {
    class HeScene;
    class HeGraphics;
    class HeTexture;
    class HeMaterial;
    class HeShader;
    class HeSceneNode;
    class HeGeometryTriangleSoup;
    class HeGeometryThickLines;

    class HeOnOff {
    public:
        void AddSceneNode(HeSceneNode* node);
        void First();
        void Last();
        void Next();
        void Previous();
        void HideAll();

    protected:
        vector<HeSceneNode*> nodes;
        size_t index = 0;
    };

    class HeVisualDebugger
    {
    public:
        HeVisualDebugger(HeScene* scene, HeGraphics* graphics);

        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2);

        void AddLine(const glm::vec3& v0, const glm::vec3& v1);
        void AddLine(const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1);

        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd);
        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, const HeColor& color);
        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, HeTexture* texture);

        void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color = HeColor::WHITE);
        void AddBox(const glm::vec3& position, float xLength = 1.0f, float yLength = 1.0f, float zLength = 1.0f, const HeColor& color = HeColor::WHITE);

        inline HeOnOff* GetOnOff() { return &onoff; }

    protected:
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

        map<HeTexture*, HeSceneNode*> texturedSceneNodes;

        HeOnOff onoff;
    };
}