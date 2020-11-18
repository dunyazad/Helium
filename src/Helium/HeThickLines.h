#pragma once

#include "HeliumCommon.h"
#include "HeGeometry.h"

namespace ArtificialNature {

	class HeThickLines : public HeGeometry
	{
	public:
		HeThickLines();
		~HeThickLines();

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
