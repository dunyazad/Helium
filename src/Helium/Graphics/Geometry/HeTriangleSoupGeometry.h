#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeTriangleSoupGeometry : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
		void AddTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec4& color);

	protected:
		HeTriangleSoupGeometry(const string& name);
		~HeTriangleSoupGeometry();

	public:
		friend class HeGraphics;
	};

}
