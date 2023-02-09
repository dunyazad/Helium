#include <Helium/Graphics/HeRay.h>

namespace ArtificialNature {

	HeRay::HeRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
		: origin(rayOrigin), direction(rayDirection)
	{
	}
}
