#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeAABB;
	class HeGeometry;

	class HeOctreeGeometry
	{
	public:
		HeOctreeGeometry(HeGeometry* geometry, const glm::vec3& vmin, const glm::vec3& vmax, int depth);
		~HeOctreeGeometry();

		void AddTriangle(int faceIndex, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

		bool IntersectRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) const;
		bool GetRayInersectingOctrees(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, vector<HeOctreeGeometry*>& intersectingOctrees);

		bool IntersectLine(const glm::vec3& p0, const glm::vec3& p1) const;
		bool GetLineInersectingOctrees(const glm::vec3& p0, const glm::vec3& p1, vector<HeOctreeGeometry*>& intersectingOctrees);

		inline const HeGeometry* GetGeometry() const { return geometry; }
		inline const HeAABB* GetAABB() const { return aabb; }
		inline const HeOctreeGeometry* const * GetSubvolumes() const { return subvolumes; }
		inline int GetDepth() const { return depth; }
		inline const vector<int>& GetContainingFaceIndices() const { return containingFaceIndices; }

	protected:
		void CreateSub(int index);
		void CreateSub(bool x, bool y, bool z);

	protected:
		HeAABB* aabb = nullptr;
		HeGeometry* geometry = nullptr;

		HeOctreeGeometry* subvolumes[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		HeAABB* subAABBs[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

		int depth = 0;

		vector<int> containingFaceIndices;
	};

	ostream& operator << (ostream& o, const HeOctreeGeometry& aabb);
	ostream& operator << (ostream& o, const HeOctreeGeometry* aabb);

}
