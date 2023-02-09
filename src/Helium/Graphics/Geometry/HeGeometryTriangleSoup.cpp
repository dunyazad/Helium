#include <Helium/Graphics/Geometry/HeGeometryTriangleSoup.h>

namespace ArtificialNature {

	HeGeometryTriangleSoup::HeGeometryTriangleSoup(const string& name)
		: HeGeometry(name)
	{
	}

	HeGeometryTriangleSoup::~HeGeometryTriangleSoup()
	{
	}

	void HeGeometryTriangleSoup::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeGeometryTriangleSoup::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeGeometryTriangleSoup::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::AddTriangle(
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
		const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		this->AddUV(uv0);
		this->AddUV(uv1);
		this->AddUV(uv2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::AddTriangle(
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
		const glm::vec3& vn0, const glm::vec3& vn1, const glm::vec3& vn2)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		this->AddNormal(vn0);
		this->AddNormal(vn1);
		this->AddNormal(vn2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::AddTriangle(
		const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
		const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
		const glm::vec3& vn0, const glm::vec3& vn1, const glm::vec3& vn2)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		this->AddUV(uv0);
		this->AddUV(uv1);
		this->AddUV(uv2);

		this->AddNormal(vn0);
		this->AddNormal(vn1);
		this->AddNormal(vn2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddColor(color);
		this->AddColor(color);
		this->AddColor(color);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const HeColor& color0, const HeColor& color1, const HeColor& color2)
	{
		int vertexCount = (int)this->GetVertexCount();

		this->AddVertex(v0);
		this->AddVertex(v1);
		this->AddVertex(v2);

		this->AddColor(color0);
		this->AddColor(color1);
		this->AddColor(color2);

		this->AddIndex(vertexCount);
		this->AddIndex(vertexCount + 1);
		this->AddIndex(vertexCount + 2);

		dirty = true;
	}

	void HeGeometryTriangleSoup::ComputeFaceNormals()
	{
		this->ClearNormals();

		auto faceCount = this->GetIndexCount();
		for (size_t i = 0; i < faceCount / 3; i++)
		{
			auto vi0 = this->GetIndex(int(i * 3));
			auto vi1 = this->GetIndex(int(i * 3 + 1));
			auto vi2 = this->GetIndex(int(i * 3 + 2));

			auto& v0 = this->GetVertex(vi0);
			auto& v1 = this->GetVertex(vi1);
			auto& v2 = this->GetVertex(vi2);

			auto d10 = glm::normalize(v0 - v1);
			auto d12 = glm::normalize(v2 - v1);
			auto fn = glm::normalize(glm::cross(d10, d12));

			this->AddNormal(fn);
			this->AddNormal(fn);
			this->AddNormal(fn);
		}
	}
}
