#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeGeometryTriangleSoup : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
		void AddTriangle(
			const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
			const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2);
		void AddTriangle(
			const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
			const glm::vec3& vn0, const glm::vec3& vn1, const glm::vec3& vn2);
		void AddTriangle(
			const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
			const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2,
			const glm::vec3& vn0, const glm::vec3& vn1, const glm::vec3& vn2);
		void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color);

		void ComputeFaceNormals();

	protected:
		HeGeometryTriangleSoup(const string& name);
		~HeGeometryTriangleSoup();

	public:
		friend class HeGraphics;
	};

}
