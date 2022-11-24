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

	bool HePlane::RayIntersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& intersection)
	{
		float distance = FLT_MAX;
		if (glm::intersectRayPlane(rayOrigin, rayDirection, position, normal, distance))
		{
			intersection = rayOrigin + rayDirection * distance;
			return true;
		}

		return false;
	}
}
