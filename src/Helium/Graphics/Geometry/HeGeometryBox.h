#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/Geometry/HeGeometry.h>

namespace ArtificialNature {

	class HeGeometryBox : public HeGeometry
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

	protected:
		HeGeometryBox(const string& name, const glm::vec3& bmin, const glm::vec3& bmax);
		HeGeometryBox(const string& name, const glm::vec3& center, float xLength, float yLength, float zLength);
		~HeGeometryBox();

	public:
		friend class HeGraphics;
	};

}
