#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeAABB
	{
	public:
		HeAABB();
		HeAABB(const HeAABB* other);
		HeAABB(const HeAABB& other);
		HeAABB(const glm::vec3& vmin, const glm::vec3& vmax);
		~HeAABB();

		void Extend(const glm::vec2& v);
		void Extend(const glm::vec3& v);

		bool Contains(const glm::vec2& v);
		bool Contains(const glm::vec3& v);
		bool Contains(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

		inline const glm::vec3& GetMin() const { return vmin; }
		inline const glm::vec3& GetMax() const { return vmax; }
		inline glm::vec3 GetCenter() const { return (vmin + vmax) * 0.5f; }
		inline glm::vec3 GetSize() const { return vmax - vmin; }

	protected:
		glm::vec3 vmin = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		glm::vec3 vmax = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	};

	ostream& operator << (ostream& o, const HeAABB& aabb);
	ostream& operator << (ostream& o, const HeAABB* aabb);

}
