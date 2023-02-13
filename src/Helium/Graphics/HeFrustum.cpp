#include <Helium/Graphics/HeFrustum.h>
#include <Helium/Graphics/HePlane.h>

namespace ArtificialNature {

	HeFrustum::HeFrustum(const glm::mat4& transform, int imageWidth, int imageHeight, float fx, float fy)
	{
		this->transform = transform;
		this->position = transform[3];
		this->rotation = transform;
		this->imageWidth = imageWidth;
		this->imageHeight = imageHeight;
		this->fx = fx;
		this->fy = fy;

		auto halfWidth = imageWidth * 0.5f;
		auto halfHeight = imageHeight * 0.5f;

		//this->fov = glm::radians(2 * atan(halfWidth / fx));
		//this->projection = glm::perspective(this->fov, (float)imageWidth / (float)imageHeight, 0.00001f, fx);
		//auto mvp = this->projection;// *view* this->transform;

		auto iCenter = glm::vec3(0, 0, fx);
		auto iLeftUp = glm::vec3(-halfWidth, halfHeight, fx);
		auto iLeftDown = glm::vec3(-halfWidth, -halfHeight, fx);
		auto iRightUp = glm::vec3(halfWidth, halfHeight, fx);
		auto iRightDown = glm::vec3(halfWidth, -halfHeight, fx);

		this->imageCenter = this->transform * glm::vec4(iCenter, 0);
		this->imageLeftUp = this->transform * glm::vec4(iLeftUp, 0);
		this->imageLeftDown = this->transform * glm::vec4(iLeftDown, 0);
		this->imageRightUp = this->transform * glm::vec4(iRightUp, 0);
		this->imageRightDown = this->transform * glm::vec4(iRightDown, 0);

		//auto ic = mvp * glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);// ic /= ic.w;
		//this->imageCenter = ic * this->transform;
		//auto ilu = mvp * glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);// ilu /= ilu.w;
		//this->imageLeftUp = ilu * this->transform;
		//auto ild = mvp * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);// ild /= ild.w;
		//this->imageLeftDown = ild * this->transform;
		//auto iru = mvp * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);// iru /= iru.w;
		//this->imageRightUp = iru * this->transform;
		//auto ird = mvp * glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);// ird /= ird.w;
		//this->imageRightDown = ird * this->transform;

		this->forward = rotation[2];
		this->right = rotation[0];
		this->up = rotation[1];

		auto dlu = glm::normalize(this->imageLeftUp - this->position);
		auto dld = glm::normalize(this->imageLeftDown - this->position);
		auto dru = glm::normalize(this->imageRightUp - this->position);
		auto drd = glm::normalize(this->imageRightDown - this->position);

		this->nearPlaneNormal = glm::normalize(this->imageCenter - this->position);
		this->leftPlaneNormal = glm::normalize(glm::cross( dlu, dld));
		this->rightPlaneNormal = glm::normalize(glm::cross(drd, dru));
		this->upperPlaneNormal = glm::normalize(glm::cross(dru, dlu));
		this->lowerPlaneNormal = glm::normalize(glm::cross(dld, drd));

		this->nearPlane = new HePlane(this->position, this->nearPlaneNormal);
		this->leftPlane = new HePlane(this->position, this->leftPlaneNormal);
		this->rightPlane = new HePlane(this->position, this->rightPlaneNormal);
		this->upperPlane = new HePlane(this->position, this->upperPlaneNormal);
		this->lowerPlane = new HePlane(this->position, this->lowerPlaneNormal);
	}

	HeFrustum::~HeFrustum()
	{
		delete nearPlane;
		delete leftPlane;
		delete rightPlane;
		delete upperPlane;
		delete lowerPlane;
	}

	bool HeFrustum::Contains(const glm::vec3& point) const
	{
		if (this->nearPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->leftPlane->PointIsOnPositiveSide(point)) return false;
		if (this->rightPlane->PointIsOnPositiveSide(point)) return false;
		if (this->upperPlane->PointIsOnPositiveSide(point)) return false;
		if (this->lowerPlane->PointIsOnPositiveSide(point)) return false;
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
