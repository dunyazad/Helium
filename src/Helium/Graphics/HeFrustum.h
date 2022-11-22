#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HePlane;

	class HeFrustum
	{
	public:
		HeFrustum(const glm::vec3& position, const glm::mat3& rotation, int imageWidth, int imageHeight, float fx, float fy);

		~HeFrustum();

		bool Contains(const glm::vec3& point) const;

		bool ContainsAny(const vector<glm::vec3>& points) const;

		bool ContainsAll(const vector<glm::vec3>& points) const;

		inline const glm::vec3& GetPosition() const { return position; }
		inline const glm::mat3& GetRotation() const { return rotation; }

		inline const glm::vec3& GetNormalizedRight() const { return nr; }
		inline const glm::vec3& GetNormalizedUp() const { return nu; }
		inline const glm::vec3& GetNormalizedFront() const { return nf; }

		inline const glm::vec3& GetLocalDirectionLeftTop() const { return ldlu; }
		inline const glm::vec3& GetLocalDirectionRightTop() const { return ldru; }
		inline const glm::vec3& GetLocalDirectionLeftBottom() const { return ldll; }
		inline const glm::vec3& GetLocalDirectionRightBottom() const { return ldrl; }

		inline const glm::vec3& GetDirectionLeftTop() const { return adlu; }
		inline const glm::vec3& GetDirectionRightTop() const { return adru; }
		inline const glm::vec3& GetDirectionLeftBottom() const { return adll; }
		inline const glm::vec3& GetDirectionRightBottom() const { return adrl; }

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

		glm::vec3 ldlu = glm::vec3(0, 0, 0);
		glm::vec3 ldru = glm::vec3(0, 0, 0);
		glm::vec3 ldll = glm::vec3(0, 0, 0);
		glm::vec3 ldrl = glm::vec3(0, 0, 0);

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
