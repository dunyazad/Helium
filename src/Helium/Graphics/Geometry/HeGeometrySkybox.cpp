#include <Helium/Graphics/Geometry/HeGeometrySkybox.h>

namespace ArtificialNature {

	HeGeometrySkybox::HeGeometrySkybox(const string& name)
		: HeGeometry(name)
	{
	}

	HeGeometrySkybox::~HeGeometrySkybox()
	{
	}

	void HeGeometrySkybox::Initialize()
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

	void HeGeometrySkybox::Terminate()
	{
		HeGeometry::Terminate();
	}

}
