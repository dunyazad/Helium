#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HePlane;

	class HeFrustum
	{
	public:
		HeFrustum(const glm::vec3& position, const glm::mat3& rotation, int imageWidth, int imageHeight, float fx, float fy);

		~HeFrustum();

		bool Contains(const glm::vec3& point);

		bool ContainsAny(const vector<glm::vec3>& points);

	protected:
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::mat3 rotation = glm::identity<glm::mat3>();
		int imageWidth = 0;
		int imageHeight = 0;
		float fx = 0.0f;
		float fy = 0.0f;

		glm::vec3 forward = glm::vec3(0, 0, 0);
		glm::vec3 right = glm::vec3(0, 0, 0);
		glm::vec3 up = glm::vec3(0, 0, 0);

		glm::vec3 leftPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 rightPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 upperPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 lowerPlaneNormal = glm::vec3(0, 0, 0);

		HePlane* leftPlane = nullptr;
		HePlane* rightPlane = nullptr;
		HePlane* upperPlane = nullptr;
		HePlane* lowerPlane = nullptr;

		glm::vec3 adlu = glm::vec3(0, 0, 0);
		glm::vec3 adru = glm::vec3(0, 0, 0);
		glm::vec3 adll = glm::vec3(0, 0, 0);
		glm::vec3 adrl = glm::vec3(0, 0, 0);

		glm::vec3 dfl = glm::vec3(0, 0, 0);
		glm::vec3 dfr = glm::vec3(0, 0, 0);
		glm::vec3 dfu = glm::vec3(0, 0, 0);
		glm::vec3 dfd = glm::vec3(0, 0, 0);

		glm::vec3 nf = glm::vec3(0, 0, 0);
		glm::vec3 nr = glm::vec3(0, 0, 0);
		glm::vec3 nu = glm::vec3(0, 0, 0);
	};

}
