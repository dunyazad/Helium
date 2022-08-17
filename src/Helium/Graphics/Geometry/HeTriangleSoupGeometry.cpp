#include <Helium/Graphics/Geometry/HeTriangleSoupGeometry.h>

namespace ArtificialNature {

	HeTriangleSoupGeometry::HeTriangleSoupGeometry(const string& name)
		: HeGeometry(name)
	{
	}

	HeTriangleSoupGeometry::~HeTriangleSoupGeometry()
	{
	}

	void HeTriangleSoupGeometry::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeTriangleSoupGeometry::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeTriangleSoupGeometry::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
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

	void HeTriangleSoupGeometry::AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color)
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
}
