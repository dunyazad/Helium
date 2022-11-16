#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HePlane
	{
	public:
		HePlane(const glm::vec3& position, const glm::vec3& normal);

		bool PointIsOnPositiveSide(const glm::vec3& point);

	protected:
		glm::vec3 position = glm::vec3();
		glm::vec3 normal = glm::vec3();
	};

}
