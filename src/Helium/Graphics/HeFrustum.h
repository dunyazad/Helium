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

		bool ContainsAny(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
		bool ContainsAny(const vector<glm::vec3>& points) const;

		bool ContainsAll(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
		bool ContainsAll(const vector<glm::vec3>& points) const;

		inline const glm::vec3& GetPosition() const { return position; }
		inline const glm::mat3& GetRotation() const { return rotation; }

		inline const glm::mat4& GetTransform() const { return transform; }

		inline const glm::vec3& GetForward() const { return this->forward; }
		inline const glm::vec3& GetRight() const { return this->right; }
		inline const glm::vec3& GetUp() const { return this->up; }

		inline const glm::vec3& GetLeftPlaneNormal() const { return this->leftPlaneNormal; }
		inline const glm::vec3& GetRightPlaneNormal() const { return this->rightPlaneNormal; }
		inline const glm::vec3& GetUpperPlaneNormal() const { return this->upperPlaneNormal; }
		inline const glm::vec3& GetLowerPlaneNormal() const { return this->lowerPlaneNormal; }

		inline const glm::vec3& GetImageCenter() const { return this->imageCenter; }
		inline const glm::vec3& GetImageLeftUp() const { return this->imageLeftUp; }
		inline const glm::vec3& GetImageLeftDown() const { return this->imageLeftDown; }
		inline const glm::vec3& GetImageRightUp() const { return this->imageRightUp; }
		inline const glm::vec3& GetImageRightDown() const { return this->imageRightDown; }

		inline HePlane* GetLeftPlane() const { return this->leftPlane; }
		inline HePlane* GetRightPlane() const { return this->rightPlane; }
		inline HePlane* GetUpperPlane() const { return this->upperPlane; }
		inline HePlane* GetLowerPlane() const { return this->lowerPlane; }

	protected:
		glm::vec3 position = glm::vec3(0, 0, 0);
		glm::mat3 rotation = glm::identity<glm::mat3>();
		int imageWidth = 0;
		int imageHeight = 0;
		float fx = 0.0f;
		float fy = 0.0f;

		glm::mat4 transform = glm::mat4();
		glm::vec3 forward = glm::vec3(0, 0, 0);
		glm::vec3 right = glm::vec3(0, 0, 0);
		glm::vec3 up = glm::vec3(0, 0, 0);

		glm::vec3 leftPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 rightPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 upperPlaneNormal = glm::vec3(0, 0, 0);
		glm::vec3 lowerPlaneNormal = glm::vec3(0, 0, 0);

		glm::vec3 imageCenter = glm::vec3(0, 0, 0);
		glm::vec3 imageLeftUp = glm::vec3(0, 0, 0);
		glm::vec3 imageLeftDown = glm::vec3(0, 0, 0);
		glm::vec3 imageRightUp = glm::vec3(0, 0, 0);
		glm::vec3 imageRightDown = glm::vec3(0, 0, 0);

		HePlane* leftPlane = nullptr;
		HePlane* rightPlane = nullptr;
		HePlane* upperPlane = nullptr;
		HePlane* lowerPlane = nullptr;
	};
}
