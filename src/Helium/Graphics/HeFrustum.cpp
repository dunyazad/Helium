#include <Helium/Graphics/HeFrustum.h>
#include <Helium/Graphics/HePlane.h>

namespace ArtificialNature {

	HeFrustum::HeFrustum(const glm::vec3& position, const glm::mat3& rotation, int imageWidth, int imageHeight, float fx, float fy)
	{
		this->position = position;
		this->rotation = rotation;
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->fx = fx;
		this->fy = fy;

		auto halfWidth = imageWidth * 0.5f;
		auto halfHeight = imageHeight * 0.5f;

		auto imageCenter = glm::vec3(0, 0, fx);
		auto imageLeftUp = imageCenter + glm::vec3(-halfWidth, halfHeight, 0);
		auto imageLeftDown = imageCenter + glm::vec3(-halfWidth, -halfHeight, 0);
		auto imageRightUp = imageCenter + glm::vec3(halfWidth, halfHeight, 0);
		auto imageRightDown = imageCenter + glm::vec3(halfWidth, -halfHeight, 0);

		this->transform = glm::mat4(rotation);
		this->transform[3] = glm::vec4(position, 1);


		this->imageCenter = this->transform * glm::vec4(imageCenter, 1);
		this->imageLeftUp = this->transform * glm::vec4(imageLeftUp, 1);
		this->imageLeftDown = this->transform * glm::vec4(imageLeftDown, 1);
		this->imageRightUp = this->transform * glm::vec4(imageRightUp, 1);
		this->imageRightDown = this->transform * glm::vec4(imageRightDown, 1);

		this->forward = rotation[2];
		this->right = rotation[0];
		this->up = rotation[1];

		auto dlu = glm::normalize(this->imageLeftUp - this->imageCenter);
		auto dld = glm::normalize(this->imageLeftDown - this->imageCenter);
		auto dru = glm::normalize(this->imageRightUp - this->imageCenter);
		auto drd = glm::normalize(this->imageRightDown - this->imageCenter);

		this->leftPlaneNormal = glm::normalize(glm::cross(dlu, dld));
		this->rightPlaneNormal = glm::normalize(glm::cross(drd, dru));
		this->upperPlaneNormal = glm::normalize(glm::cross(dru, dlu));
		this->lowerPlaneNormal = glm::normalize(glm::cross(dld, drd));

		this->leftPlane = new HePlane(this->imageCenter, this->leftPlaneNormal);
		this->rightPlane = new HePlane(this->imageCenter, this->rightPlaneNormal);
		this->upperPlane = new HePlane(this->imageCenter, this->upperPlaneNormal);
		this->lowerPlane = new HePlane(this->imageCenter, this->lowerPlaneNormal);
	}

	HeFrustum::~HeFrustum()
	{
		delete leftPlane;
		delete rightPlane;
		delete upperPlane;
		delete lowerPlane;
	}

	bool HeFrustum::Contains(const glm::vec3& point) const
	{
		if (this->leftPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->rightPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->upperPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->lowerPlane->PointIsOnPositiveSide(point) == false) return false;
		return true;
	}

	bool HeFrustum::ContainsAny(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const
	{
		if (Contains(v0) == true)
			return true;
		if (Contains(v1) == true)
			return true;
		if (Contains(v2) == true)
			return true;

		return false;
	}

	bool HeFrustum::ContainsAny(const vector<glm::vec3>& points) const
	{
		for (auto& point : points)
		{
			if (Contains(point) == true)
			{
				return true;
			}
		}

		return false;
	}

	bool HeFrustum::ContainsAll(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const
	{
		if (Contains(v0) == false)
			return false;
		if (Contains(v1) == false)
			return false;
		if (Contains(v2) == false)
			return false;

		return true;
	}

	bool HeFrustum::ContainsAll(const vector<glm::vec3>& points) const
	{
		for (auto& point : points)
		{
			if (Contains(point) == false)
			{
				return false;
			}
		}

		return true;
	}
}
