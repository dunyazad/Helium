#pragma once

#include <Helium/Helium.h>

namespace ArtificialNature {

    class HeVisualDebugger
    {
    public:
        HeVisualDebugger(Helium& helium);

        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2);

        void AddLine(const glm::vec3& v0, const glm::vec3& v1);

        void AddLine(const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1);

        void AddBox(const glm::vec3& bmin, const glm::vec3& bmax);

        void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color);

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
}