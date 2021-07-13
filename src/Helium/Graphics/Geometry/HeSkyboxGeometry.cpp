#include <Helium/Graphics/Geometry/HeSkyboxGeometry.h>

namespace ArtificialNature {

	HeSkyboxGeometry::HeSkyboxGeometry(const string& name)
		: HeGeometry(name)
	{
	}

	HeSkyboxGeometry::~HeSkyboxGeometry()
	{
	}

	void HeSkyboxGeometry::Initialize()
	{
		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));

		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));

		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));

		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));

		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, 1.0f));
		AddVertex(glm::vec3(-1.0f, 1.0f, -1.0f));

		AddVertex(glm::vec3(-1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, -1.0f));
		AddVertex(glm::vec3(-1.0f, -1.0f, 1.0f));
		AddVertex(glm::vec3(1.0f, -1.0f, 1.0f));

		HeGeometry::Initialize();
	}

	void HeSkyboxGeometry::Terminate()
	{
		HeGeometry::Terminate();
	}

}
