#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HePlane
	{
	public:
		HePlane(const glm::vec3& position, const glm::vec3& normal);

		bool PointIsOnPositiveSide(const glm::vec3& point);

		bool RayIntersects(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, glm::vec3& intersection);

		bool LineIntersects(const glm::vec3& p0, const glm::vec3& p1, glm::vec3& intersection);
	protected:
		glm::vec3 position = glm::vec3();
		glm::vec3 normal = glm::vec3();
	};

}
