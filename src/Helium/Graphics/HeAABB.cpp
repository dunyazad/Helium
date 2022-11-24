#include <Helium/Graphics/HeAABB.h>

namespace ArtificialNature {

	HeAABB::HeAABB() {}
	
	HeAABB::HeAABB(const HeAABB* other)
	{
		vmin = other->vmin;
		vmax = other->vmax;
	}

	HeAABB::HeAABB(const HeAABB& other)
	{
		vmin = other.vmin;
		vmax = other.vmax;
	}

	HeAABB::HeAABB(const glm::vec3& vmin, const glm::vec3& vmax)
		: vmin(vmin), vmax(vmax)
	{
	}

	HeAABB::~HeAABB() {}

	void HeAABB::Extend(const glm::vec2& v)
	{
		if (v.x < vmin.x) vmin.x = v.x;
		if (v.y < vmin.y) vmin.y = v.y;

		if (v.x > vmax.x) vmax.x = v.x;
		if (v.y > vmax.y) vmax.y = v.y;
	}

	void HeAABB::Extend(const glm::vec3& v)
	{
		if (v.x < vmin.x) vmin.x = v.x;
		if (v.y < vmin.y) vmin.y = v.y;
		if (v.z < vmin.z) vmin.z = v.z;

		if (v.x > vmax.x) vmax.x = v.x;
		if (v.y > vmax.y) vmax.y = v.y;
		if (v.z > vmax.z) vmax.z = v.z;
	}

	bool HeAABB::Contains(const glm::vec2& v)
	{
		if (vmin.x > v.x || vmax.x < v.x)
			return false;
		if (vmin.y > v.y || vmax.y < v.y)
			return false;

		return true;
	}
	
	bool HeAABB::Contains(const glm::vec3& v)
	{
		if (vmin.x > v.x || vmax.x < v.x)
			return false;
		if (vmin.y > v.y || vmax.y < v.y)
			return false;
		if (vmin.z > v.z || vmax.z < v.z)
			return false;

		return true;
	}

	bool HeAABB::Contains(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		if (Contains(v0) == false)
			return false;
		if (Contains(v1) == false)
			return false;
		if (Contains(v2) == false)
			return false;

		return true;
	}

	ostream& operator << (ostream& o, const HeAABB& aabb)
	{
		return o << "min:\t" << aabb.GetMin() << endl << "max:\t" << aabb.GetMax() << endl << "center:\t" << aabb.GetCenter() << endl << "size:\t" << aabb.GetSize() << endl;
	}

	ostream& operator << (ostream& o, const HeAABB* aabb)
	{
		return o << "min:\t" << aabb->GetMin() << endl << "max:\t" << aabb->GetMax() << endl << "center:\t" << aabb->GetCenter() << endl << "size:\t" << aabb->GetSize() << endl;
	}
}
