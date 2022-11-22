#include <Helium/Graphics/HePlane.h>

namespace ArtificialNature {

	HePlane::HePlane(const glm::vec3& position, const glm::vec3& normal)
		: position(position), normal(normal)
	{
	}

	bool HePlane::PointIsOnPositiveSide(const glm::vec3& point)
	{
		auto direction = point - this->position;
		return glm::dot(direction, this->normal) > 0;
	}

}
