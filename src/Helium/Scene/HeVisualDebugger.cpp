#include <Helium/Scene/HeVisualDebugger.h>
#include <Helium/Scene/Scene.h>
#include <Helium/Graphics/Graphics.h>

namespace ArtificialNature {
    
    void HeOnOff::AddSceneNode(HeSceneNode* node) {
        nodes.push_back(node);
    }

    void HeOnOff::First()
    {
        nodes[index]->SetActive(false);
        nodes[0]->SetActive(true);
        index = 0;
    }

    void HeOnOff::Last()
    {
        nodes[index]->SetActive(false);
        nodes[nodes.size() - 1]->SetActive(true);
        index = nodes.size() - 1;
    }

    void HeOnOff::Next()
    {
        if (index + 1 < nodes.size()) {
            HideAll();

            nodes[index]->SetActive(false);
            nodes[index + 1]->SetActive(true);
            index++;
        }
    }

    void HeOnOff::Previous()
    {
        if (index > 0) {
            HideAll();

            nodes[index]->SetActive(false);
            nodes[index - 1]->SetActive(true);
            index--;
        }
    }

    void HeOnOff::HideAll()
    {
        for (auto& n : nodes)
        {
            n->SetActive(false);
        }
    }

    HeVisualDebugger::HeVisualDebugger(HeScene* scene, HeGraphics* graphics)
        : scene(scene), graphics(graphics)
    {
        solidSceneNode = scene->CreateSceneNode("Visual Debugger.SolidSceneNode");
        solidGeometry = graphics->GetGeometryTriangleSoup("Visual Debugger.SolidGeometry");
        solidGeometry->Initialize();
        solidSceneNode->AddGeometry(solidGeometry);
        solidMaterial = graphics->GetMaterial("Visual Debugger.SolidMaterial");
        solidShader = graphics->GetShader("Visual Debugger.SolidShader", "../../res/shader/vertexColor.vs", "../../res/shader/vertexColor.fs");
        solidMaterial->SetShader(solidShader);
        solidGeometry->SetMaterial(solidMaterial);

        lineSceneNode = scene->CreateSceneNode("Visual Debugger.LineSceneNode");
        lineGeometry = graphics->GetGeometryThickLines("Visual Debugger.LineGeometry");
        lineGeometry->Initialize();
        lineSceneNode->AddGeometry(lineGeometry);
        lineMaterial = graphics->GetMaterial("Visual Debugger.LineMaterial");
        lineShader = graphics->GetShader("Visual Debugger.LineShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
        lineMaterial->SetShader(lineShader);
        lineGeometry->SetMaterial(lineMaterial);
        lineGeometry->SetThickness(1);
        lineGeometry->SetDrawingMode(HeGeometry::DrawingMode::Lines);

        axisSceneNode = scene->CreateSceneNode("Visual Debugger.AxisSceneNode");
        axisGeometry = graphics->GetGeometryThickLines("Visual Debugger.AxisGeometry");
        axisGeometry->Initialize();
        axisSceneNode->AddGeometry(axisGeometry);
        axisMaterial = graphics->GetMaterial("Visual Debugger.AxisMaterial");
        axisShader = graphics->GetShader("Visual Debugger.AxisShader", "../../res/shader/thick lines.vs", "../../res/shader/thick lines.fs");
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

        axisGeometry->AddColor(HeColor::RED);
        axisGeometry->AddColor(HeColor::RED);
        axisGeometry->AddColor(HeColor::GREEN);
        axisGeometry->AddColor(HeColor::GREEN);
        axisGeometry->AddColor(HeColor::BLUE);
        axisGeometry->AddColor(HeColor::BLUE);

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

    void HeVisualDebugger::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
    {
        AddTriangle(v0, v1, v2, HeColor::WHITE, HeColor::WHITE, HeColor::WHITE);
    }

    void HeVisualDebugger::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2)
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

    void HeVisualDebugger::AddLine(const glm::vec3& v0, const glm::vec3& v1)
    {
        AddLine(v0, v1, HeColor::WHITE, HeColor::WHITE);
    }

    void HeVisualDebugger::AddLine(const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1)
    {
        int vertexCount = (int)lineGeometry->GetVertexCount();

        lineGeometry->AddVertex(v0);
        lineGeometry->AddVertex(v1);

        lineGeometry->AddIndex(vertexCount);
        lineGeometry->AddIndex(vertexCount + 1);

        lineGeometry->AddColor(color0);
        lineGeometry->AddColor(color1);
    }

    void HeVisualDebugger::AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd)
    {
        AddPlane(lu, ld, ru, rd, HeColor::WHITE);
    }

    void HeVisualDebugger::AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, const HeColor& color)
    {
        int vertexCount = (int)lineGeometry->GetVertexCount();

        lineGeometry->AddVertex(lu);
        lineGeometry->AddVertex(ld);
        lineGeometry->AddVertex(ru);
        lineGeometry->AddVertex(rd);

        lineGeometry->AddIndex(vertexCount);
        lineGeometry->AddIndex(vertexCount + 1);
        lineGeometry->AddIndex(vertexCount + 2);
        lineGeometry->AddIndex(vertexCount + 3);

        lineGeometry->AddColor(color);
        lineGeometry->AddColor(color);
        lineGeometry->AddColor(color);
        lineGeometry->AddColor(color);

        solidGeometry->AddTriangle(ld, lu, rd, color);
        solidGeometry->AddTriangle(rd, lu, ru, color);
    }

    void HeVisualDebugger::AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, HeTexture* texture)
    {
        int vertexCount = (int)lineGeometry->GetVertexCount();

        //lineGeometry->AddVertex(lu);
        //lineGeometry->AddVertex(ld);
        //lineGeometry->AddVertex(ru);
        //lineGeometry->AddVertex(rd);

        //lineGeometry->AddIndex(vertexCount);
        //lineGeometry->AddIndex(vertexCount + 1);
        //lineGeometry->AddIndex(vertexCount + 1);
        //lineGeometry->AddIndex(vertexCount + 3);
        //lineGeometry->AddIndex(vertexCount + 3);
        //lineGeometry->AddIndex(vertexCount + 2);
        //lineGeometry->AddIndex(vertexCount + 2);
        //lineGeometry->AddIndex(vertexCount);
        
        //lineGeometry->AddColor(HeColor::WHITE);
        //lineGeometry->AddColor(HeColor::WHITE);
        //lineGeometry->AddColor(HeColor::WHITE);
        //lineGeometry->AddColor(HeColor::WHITE);

        if (texturedSceneNodes.count(texture) == 0)
        {
            auto pNode = scene->CreateSceneNode("Visual Debugger.TexturedSceneNode " + texture->GetName());
            texturedSceneNodes[texture] = pNode;
            onoff.AddSceneNode(pNode);

            auto pGeometry = graphics->GetGeometryTriangleSoup("Visual Debugger.TexturedGeometry " + texture->GetName());
            pGeometry->Initialize();
            pNode->AddGeometry(pGeometry);
            auto pMaterial = graphics->GetMaterialSingleTexture("Visual Debugger.TexturedMaterial " + texture->GetName());
            pMaterial->SetTexture(texture);
            auto pShader = graphics->GetShader("Visual Debugger.TexturedShader", "../../res/shader/texture.vs", "../../res/shader/texture.fs");
            pMaterial->SetShader(pShader);
            pGeometry->SetMaterial(pMaterial);
        }

        auto pNode = texturedSceneNodes[texture];
        auto pGeometry = dynamic_cast<HeGeometryTriangleSoup*>(pNode->GetGeometry());
        pGeometry->AddTriangle(ld, lu, rd, glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 0));
        pGeometry->AddTriangle(rd, lu, ru, glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(1, 1));
    }

    void HeVisualDebugger::AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color)
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

    void HeVisualDebugger::AddBox(const glm::vec3& position, float xLength, float yLength, float zLength, const HeColor& color)
    {
        auto bmin = position + glm::vec3(-xLength * 0.5f, -yLength * 0.5f, -zLength * 0.5f);
        auto bmax = position + glm::vec3(xLength * 0.5f, yLength * 0.5f, zLength * 0.5f);
        AddBox(bmin, bmax, color);
    }
}
