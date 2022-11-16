#include <Helium/Graphics/HeAABB.h>

namespace ArtificialNature {

	HeAABB::HeAABB() {}
	HeAABB::~HeAABB() {}

	void HeAABB::Extend(const glm::vec3& v)
	{
		if (v.x < vmin.x) vmin.x = v.x;
		if (v.y < vmin.y) vmin.y = v.y;
		if (v.z < vmin.z) vmin.z = v.z;

		if (v.x > vmax.x) vmax.x = v.x;
		if (v.y > vmax.y) vmax.y = v.y;
		if (v.z > vmax.z) vmax.z = v.z;
	}

	ostream& operator << (ostream& o, const HeAABB& aabb)
	{
		return o << "min:\t" << aabb.GetMin() << endl << "max:\t" << aabb.GetMax() << endl << "center:\t" << aabb.GetCenter() << endl << "size:\t" << aabb.GetSize() << endl;
	}
}
