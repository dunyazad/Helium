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

		auto dlu = glm::normalize(this->forward - this->right + this->up);
		auto dru = glm::normalize(this->forward + this->right + this->up);
		auto dll = glm::normalize(this->forward - this->right - this->up);
		auto drl = glm::normalize(this->forward + this->right - this->up);

		auto nl = glm::normalize(glm::cross(dlu, dll));
		auto nr = glm::normalize(glm::cross(drl, dru));
		auto nu = glm::normalize(glm::cross(dru, dlu));
		auto nd = glm::normalize(glm::cross(dll, drl));

		this->leftPlaneNormal = this->rotation * nl;
		this->rightPlaneNormal = this->rotation * nr;
		this->upperPlaneNormal = this->rotation * nu;
		this->lowerPlaneNormal = this->rotation * nd;

		this->leftPlane = new HePlane(this->position, this->leftPlaneNormal);
		this->rightPlane = new HePlane(this->position, this->rightPlaneNormal);
		this->upperPlane = new HePlane(this->position, this->upperPlaneNormal);
		this->lowerPlane = new HePlane(this->position, this->lowerPlaneNormal);

		this->adlu = this->rotation * dlu;
		this->adru = this->rotation * dru;
		this->adll = this->rotation * dll;
		this->adrl = this->rotation * drl;

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

	bool HeFrustum::Contains(const glm::vec3& point)
	{
		if (this->leftPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->rightPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->upperPlane->PointIsOnPositiveSide(point) == false) return false;
		if (this->lowerPlane->PointIsOnPositiveSide(point) == false) return false;
		return true;
	}

	bool HeFrustum::ContainsAny(const vector<glm::vec3>& points)
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

}
