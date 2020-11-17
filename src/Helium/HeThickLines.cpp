#include "HeThickLines.h"

namespace ArtificialNature {

	HeThickLines::HeThickLines()
		: HeGeometry()
	{
	}

	HeThickLines::~HeThickLines()
	{
	}

	void HeThickLines::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeThickLines::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeThickLines::AddPoint(const glm::vec2& point)
	{
		if (points.size() == 0)
		{
			points.push_back(point);
		}
		else if (points.size() == 1)
		{
			points.push_back(point);

			auto p0 = glm::vec3(points[0].x, points[0].y, 0);
			auto p1 = glm::vec3(points[1].x, points[1].y, 0);

			auto direction = glm::normalize(p1 - p0);
			auto up = glm::normalize(glm::vec3(0, 0, 1));
			auto cross = glm::cross(up, direction);

			auto v0 = p0 + cross * thickness * 0.5f;
			auto v1 = p0 - cross * thickness * 0.5f;
			auto v2 = p1 + cross * thickness * 0.5f;
			auto v3 = p1 - cross * thickness * 0.5f;

			AddVertex(v0);
			AddVertex(v1);
			AddVertex(v2);
			AddVertex(v3);

			AddIndex(0);
			AddIndex(1);
			AddIndex(2);

			AddIndex(2);
			AddIndex(1);
			AddIndex(3);

			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			AddUV(glm::vec2(0.0f, 1.0f));
			AddUV(glm::vec2(0.0f, 0.0f));
			AddUV(glm::vec2(1.0f, 1.0f));
			AddUV(glm::vec2(1.0f, 0.0f));
		}
		else
		{

		}
	}

	void HeThickLines::Upload()
	{
		HeGeometry::Upload();
	}
}
