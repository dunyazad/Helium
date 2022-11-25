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

		this->forward = glm::vec3(0, 0, fx);
		this->right = glm::vec3(halfWidth, 0, 0);
		this->up = glm::vec3(0, halfHeight, 0);

		auto dfl = glm::normalize(this->forward - this->right);
		auto dfr = glm::normalize(this->forward + this->right);
		auto dfu = glm::normalize(this->forward + this->up);
		auto dfd = glm::normalize(this->forward - this->up);

		ldlu = glm::normalize(this->forward - this->right + this->up);
		ldru = glm::normalize(this->forward + this->right + this->up);
		ldll = glm::normalize(this->forward - this->right - this->up);
		ldrl = glm::normalize(this->forward + this->right - this->up);

		auto nl = glm::normalize(glm::cross(ldlu, ldll));
		auto nr = glm::normalize(glm::cross(ldrl, ldru));
		auto nu = glm::normalize(glm::cross(ldru, ldlu));
		auto nd = glm::normalize(glm::cross(ldll, ldrl));

		this->leftPlaneNormal = this->rotation * nl;
		this->rightPlaneNormal = this->rotation * nr;
		this->upperPlaneNormal = this->rotation * nu;
		this->lowerPlaneNormal = this->rotation * nd;

		this->leftPlane = new HePlane(this->position, this->leftPlaneNormal);
		this->rightPlane = new HePlane(this->position, this->rightPlaneNormal);
		this->upperPlane = new HePlane(this->position, this->upperPlaneNormal);
		this->lowerPlane = new HePlane(this->position, this->lowerPlaneNormal);

		this->adlu = this->rotation * ldlu;
		this->adru = this->rotation * ldru;
		this->adll = this->rotation * ldll;
		this->adrl = this->rotation * ldrl;

		this->dfl = this->rotation * dfl;
		this->dfr = this->rotation * dfr;
		this->dfu = this->rotation * dfu;
		this->dfd = this->rotation * dfd;

		this->nf = this->rotation * glm::normalize(this->forward);
		this->nr = this->rotation * glm::normalize(this->right);
		this->nu = this->rotation * glm::normalize(this->up);
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
