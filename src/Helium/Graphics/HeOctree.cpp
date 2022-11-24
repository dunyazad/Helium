#include <Helium/Graphics/HeOctree.h>
#include <Helium/Graphics/HeAABB.h>
#include <Helium/Graphics/HePlane.h>
#include <Helium/Graphics/Geometry/Geometry.h>

namespace ArtificialNature {

	HeOctreeGeometry::HeOctreeGeometry(HeGeometry* geometry, const glm::vec3& vmin, const glm::vec3& vmax, int depth)
		: geometry(geometry), depth(depth)
	{
		aabb = new HeAABB(vmin, vmax);
		auto vcenter = aabb->GetCenter();

		subAABBs[0] = new HeAABB(glm::vec3(vmin.x, vmin.y, vmin.z), glm::vec3(vcenter.x, vcenter.y, vcenter.z));
		subAABBs[1] = new HeAABB(glm::vec3(vcenter.x, vmin.y, vmin.z), glm::vec3(vmax.x, vcenter.y, vcenter.z));
		subAABBs[2] = new HeAABB(glm::vec3(vmin.x, vcenter.y, vmin.z), glm::vec3(vcenter.x, vmax.y, vcenter.z));
		subAABBs[3] = new HeAABB(glm::vec3(vcenter.x, vcenter.y, vmin.z), glm::vec3(vmax.x, vmax.y, vcenter.z));
		subAABBs[4] = new HeAABB(glm::vec3(vmin.x, vmin.y, vcenter.z), glm::vec3(vcenter.x, vcenter.y, vmax.z));
		subAABBs[5] = new HeAABB(glm::vec3(vcenter.x, vmin.y, vcenter.z), glm::vec3(vmax.x, vcenter.y, vmax.z));
		subAABBs[6] = new HeAABB(glm::vec3(vmin.x, vcenter.y, vcenter.z), glm::vec3(vcenter.x, vmax.y, vmax.z));
		subAABBs[7] = new HeAABB(glm::vec3(vcenter.x, vcenter.y, vcenter.z), glm::vec3(vmax.x, vmax.y, vmax.z));

		for (size_t i = 0; i < 8; i++)
		{
			subvolumes[i] = nullptr;
		}
	}

	HeOctreeGeometry::~HeOctreeGeometry()
	{
		if (aabb != nullptr) delete aabb;

		for (size_t i = 0; i < 8; i++)
		{
			if (subAABBs[i] != nullptr)
				delete subAABBs[i];
		}

		for (size_t i = 0; i < 8; i++)
		{
			if (subvolumes[i] != nullptr)
				delete subvolumes[i];
		}
	}

	void HeOctreeGeometry::AddTriangle(int faceIndex, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		bool subInserted = false;
		for (int i = 0; i < 8; i++)
		{
			if (subAABBs[i]->Contains(v0, v1, v2)) {
				if (subvolumes[i] == nullptr) {
					CreateSub(i);
				}
				subvolumes[i]->AddTriangle(faceIndex, v0, v1, v2);
				subInserted = true;
				break;
			}
		}

		if (subInserted == false) {
			containingFaceIndices.push_back(faceIndex);
		}
	}

	bool HeOctreeGeometry::IntersectRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const
	{
		if (aabb->Contains(rayOrigin)) {
			return true;
		}

		auto vCenter = aabb->GetCenter();
		const auto& vmin = aabb->GetMin();
		const auto& vmax = aabb->GetMax();
		bool positiveX = vCenter.x < rayOrigin.x;
		bool positiveY = vCenter.y < rayOrigin.y;
		bool positiveZ = vCenter.z < rayOrigin.z;
		auto nearPoint = glm::vec3(positiveX ? vmax.x : vmin.x, positiveY ? vmax.y : vmin.y, positiveZ ? vmax.z : vmin.z);
		HePlane planeX(nearPoint, positiveX ? glm::vec3(1, 0, 0) : glm::vec3(-1, 0, 0));
		HePlane planeY(nearPoint, positiveY ? glm::vec3(0, 1, 0) : glm::vec3(0, -1, 0));
		HePlane planeZ(nearPoint, positiveZ ? glm::vec3(0, 0, 1) : glm::vec3(0, 0, -1));

		glm::vec3 planeXIntersection;
		glm::vec3 planeYIntersection;
		glm::vec3 planeZIntersection;

		bool intersectX = false;
		bool intersectY = false;
		bool intersectZ = false;

		if (planeX.RayIntersects(rayOrigin, rayDirection, planeXIntersection))
		{
			intersectX = (vmin.y <= planeXIntersection.y && planeXIntersection.y <= vmax.y) &&
				(vmin.z <= planeXIntersection.z && planeXIntersection.z <= vmax.z);
		}

		if (planeY.RayIntersects(rayOrigin, rayDirection, planeYIntersection))
		{
			intersectY = (vmin.x <= planeYIntersection.x && planeYIntersection.x <= vmax.x) &&
				(vmin.z <= planeYIntersection.z && planeYIntersection.z <= vmax.z);
		}

		if (planeZ.RayIntersects(rayOrigin, rayDirection, planeZIntersection))
		{
			intersectZ = (vmin.x <= planeZIntersection.x && planeZIntersection.x <= vmax.x) &&
				(vmin.y <= planeZIntersection.y && planeZIntersection.y <= vmax.y);
		}

		return intersectX || intersectY || intersectZ;
	}

	bool HeOctreeGeometry::GetRayInersectingOctrees(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, vector<HeOctreeGeometry*>& intersectingOctrees)
	{
		if (IntersectRay(rayOrigin, rayDirection) == false)
		{
			return false;
		}
		else
		{
			intersectingOctrees.push_back(this);

			for (int i = 0; i < 8; i++)
			{
				if (subvolumes[i] != nullptr)
				{
					subvolumes[i]->GetRayInersectingOctrees(rayOrigin, rayDirection, intersectingOctrees);
				}
			}

			return true;
		}
	}

	void HeOctreeGeometry::CreateSub(int index)
	{
		bool x = index & 0b0001;
		bool y = index & 0b0010;
		bool z = index & 0b0100;
		CreateSub(x, y, z);
	}

	void HeOctreeGeometry::CreateSub(bool x, bool y, bool z)
	{
		const auto& vmin = aabb->GetMin();
		const auto& vmax = aabb->GetMax();
		auto vcenter = aabb->GetCenter();

		int index = x + (y << 1) + (z << 2);
		if (subvolumes[index] == nullptr) {
			subvolumes[index] = new HeOctreeGeometry(
				geometry,
				glm::vec3(x ? vcenter.x : vmin.x, y ? vcenter.y : vmin.y, z ? vcenter.z : vmin.z),
				glm::vec3(x ? vmax.x : vcenter.x, y ? vmax.y : vcenter.y, z ? vmax.z : vcenter.z),
				depth + 1
			);
		}
	}

	ostream& operator << (ostream& o, const HeOctreeGeometry& aabb)
	{
		return o;// << "min:\t" << aabb.GetMin() << endl << "max:\t" << aabb.GetMax() << endl << "center:\t" << aabb.GetCenter() << endl << "size:\t" << aabb.GetSize() << endl;
	}

	ostream& operator << (ostream& o, const HeOctreeGeometry* aabb)
	{
		return o;// << "min:\t" << aabb->GetMin() << endl << "max:\t" << aabb->GetMax() << endl << "center:\t" << aabb->GetCenter() << endl << "size:\t" << aabb->GetSize() << endl;
	}
}
