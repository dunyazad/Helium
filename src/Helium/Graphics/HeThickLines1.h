#pragma once

#include <Helium/Core/Core.h>

#include <Helium/Graphics/HeGeometry.h>

namespace ArtificialNature {

	class HeThickLines1 : public HeGeometry
	{
	public:
		HeThickLines1();
		~HeThickLines1();

		void Initialize();
		void Terminate();

		void AddPoint(const glm::vec2& point);

		inline void SetThickness(float thickness) { this->thickness = thickness; }

	protected:
		float thickness = 0.1f;

		vector<glm::vec2> points;

		float lastIndexP0;
		float lastUVP0;
	};

}
