#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeRay
	{
	public:
		HeRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

		inline const glm::vec3& GetOrigin() const { return origin; }
		inline const glm::vec3& GetDirection() const { return direction; }

	protected:
		glm::vec3 origin;
		glm::vec3 direction;
	};
}
