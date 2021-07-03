#include <Helium/Graphics/HeThickLines1.h>

#include <Helium/Graphics/HeVertexBufferObject.hpp>

namespace ArtificialNature {

	HeThickLines1::HeThickLines1()
		: HeGeometry()
	{
	}

	HeThickLines1::~HeThickLines1()
	{
	}

	void HeThickLines1::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeThickLines1::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeThickLines1::AddPoint(const glm::vec2& point)
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
			AddVertex(v3);
			AddVertex(v2);

			AddIndex(0);
			AddIndex(1);
			AddIndex(2);

			AddIndex(3);
			AddIndex(1);
			AddIndex(2);

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
			points.push_back(point);
			auto ic = points.size() - 1;
			auto ip = points.size() - 2;
			auto ipp = points.size() - 3;

			auto pc = glm::vec3(points[ic].x, points[ic].y, 0);
			auto pp = glm::vec3(points[ip].x, points[ip].y, 0);
			auto ppp = glm::vec3(points[ipp].x, points[ipp].y, 0);

			auto dpp2p = glm::normalize(ppp - pp);
			auto dp2c = glm::normalize(pp - pc);
			auto up = glm::normalize(glm::vec3(0, 0, 1));
			
			auto cpp2p = glm::cross(up, dpp2p);
			auto cp2c = glm::cross(up, dp2c);
			//if (glm::dot(dpp2p, dp2c) <= 0)
			//	cpp2p = -cpp2p;

			auto ccc = glm::normalize(cpp2p + cp2c);

			auto v0 = pp + ccc * thickness * 0.5f;
			auto v1 = pp - ccc * thickness * 0.5f;
			auto v2 = pc + cp2c * thickness * 0.5f;
			auto v3 = pc - cp2c * thickness * 0.5f;

			AddVertex(v2);
			AddVertex(v3);

			auto vi0 = vbo->Size() - 4;
			auto vi1 = vbo->Size() - 3;
			auto vi2 = vbo->Size() - 2;
			auto vi3 = vbo->Size() - 1;

			SetVertex(vi0, v0);
			SetVertex(vi1, v1);

			AddIndex(vi0);
			AddIndex(vi1);
			AddIndex(vi2);

			AddIndex(vi2);
			AddIndex(vi1);
			AddIndex(vi3);

			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			AddColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			AddUV(glm::vec2(0.0f, 1.0f));
			AddUV(glm::vec2(0.0f, 0.0f));
			AddUV(glm::vec2(1.0f, 1.0f));
			AddUV(glm::vec2(1.0f, 0.0f));
		}
	}

}
