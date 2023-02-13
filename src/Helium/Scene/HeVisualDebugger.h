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

    class HePrimitivePresenter
    {
    public:
        HePrimitivePresenter(const string& name, HeScene* scene, HeGraphics* graphics);

        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color);
        void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2);

        void AddLine(const glm::vec3& v0, const glm::vec3& v1);
        void AddLine(const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1);

        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd);
        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, const HeColor& color);
        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, HeTexture* texture);

        void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color = HeColor::WHITE);
        void AddBox(const glm::vec3& position, float xLength = 1.0f, float yLength = 1.0f, float zLength = 1.0f, const HeColor& color = HeColor::WHITE);

        void AddAxisLines(const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE);
        void AddHalfAxisLines(const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE);

        inline const string& GetName() const { return name; }
        inline HeSceneNode* GetSceneNode() { return sceneNode; }

    protected:
        string name;
        HeScene* scene = nullptr;
        HeSceneNode* sceneNode = nullptr;
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
    };

    class HeVisualDebugger
    {
    public:
        HeVisualDebugger(const string& name, HeScene* scene, HeGraphics* graphics);
        ~HeVisualDebugger();

        inline const string& GetName() const { return name; }

        inline void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
			{
				group->AddTriangle(v0, v1, v2);
			}
		}
        
        inline void AddTriangle(const string& groupName, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
		{
			auto group = GetGroup(groupName);
            if (group != nullptr)
			{
				group->AddTriangle(v0, v1, v2);
			}
		}
        
        inline void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddTriangle(v0, v1, v2, color);
            }
        }

        inline void AddTriangle(const string& groupName, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddTriangle(v0, v1, v2, color);
            }
        }

        inline void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddTriangle(v0, v1, v2, color0, color1, color2);
            }
        }

        inline void AddTriangle(const string& groupName, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
				group->AddTriangle(v0, v1, v2, color0, color1, color2);
			}
		}

        void AddLine(const string& groupName, const glm::vec3& v0, const glm::vec3& v1)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddLine(v0, v1);
            }
        }

        void AddLine(const glm::vec3& v0, const glm::vec3& v1)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddLine(v0, v1);
            }
        }

        void AddLine(const string& groupName, const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddLine(v0, v1, color0, color1);
            }
        }

        void AddLine(const glm::vec3& v0, const glm::vec3& v1, const HeColor& color0, const HeColor& color1)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddLine(v0, v1, color0, color1);
            }
        }

        void AddPlane(const string& groupName, const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd);
            }
        }

        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd);
            }
        }

        void AddPlane(const string& groupName, const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, const HeColor& color)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd, color);
            }
        }

        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, const HeColor& color)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd, color);
            }
        }

        void AddPlane(const string& groupName, const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, HeTexture* texture)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd, texture);
            }
        }

        void AddPlane(const glm::vec3& lu, const glm::vec3& ld, const glm::vec3& ru, const glm::vec3& rd, HeTexture* texture)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddPlane(lu, ld, ru, rd, texture);
            }
        }

        void AddBox(const string& groupName, const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color = HeColor::WHITE)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddBox(bmin, bmax, color);
            }
        }

        void AddBox(const glm::vec3& bmin, const glm::vec3& bmax, const HeColor& color = HeColor::WHITE)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddBox(bmin, bmax, color);
            }
        }

        void AddBox(const string& groupName, const glm::vec3& position, float xLength = 1.0f, float yLength = 1.0f, float zLength = 1.0f, const HeColor& color = HeColor::WHITE)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddBox(position, xLength, yLength, zLength, color);
            }
        }

        void AddBox(const glm::vec3& position, float xLength = 1.0f, float yLength = 1.0f, float zLength = 1.0f, const HeColor& color = HeColor::WHITE)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddBox(position, xLength, yLength, zLength, color);
            }
        }

        void AddAxisLines(const string& groupName, const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddAxisLines(transform, xLenth, yLenth, zLenth, xColor, yColor, zColor);
            }
        }

        void AddAxisLines(const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddAxisLines(transform, xLenth, yLenth, zLenth, xColor, yColor, zColor);
            }
        }

        void AddHalfAxisLines(const string& groupName, const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE)
        {
            auto group = GetGroup(groupName);
            if (group != nullptr)
            {
                group->AddHalfAxisLines(transform, xLenth, yLenth, zLenth, xColor, yColor, zColor);
            }
        }

        void AddHalfAxisLines(const glm::mat4& transform = glm::identity<glm::mat4>(), float xLenth = 1024.0f, float yLenth = 1024.0f, float zLenth = 1024.0f, const HeColor& xColor = HeColor::RED, const HeColor& yColor = HeColor::GREEN, const HeColor& zColor = HeColor::BLUE)
        {
            auto group = GetGroup("Default");
            if (group != nullptr)
            {
                group->AddHalfAxisLines(transform, xLenth, yLenth, zLenth, xColor, yColor, zColor);
            }
        }

        inline HePrimitivePresenter* GetGroup(const string& groupName)
        {
            if (groups.count(groupName) == 0)
            {
                auto group = new HePrimitivePresenter(groupName + ".PrimitivePresenter", this->scene, this->graphics);
                groups[groupName] = group;
            }
            return groups[groupName];
        }

    protected:
        string name;
        HeScene* scene = nullptr;
        HeGraphics* graphics = nullptr;
        map<string, HePrimitivePresenter*> groups;
    };
}