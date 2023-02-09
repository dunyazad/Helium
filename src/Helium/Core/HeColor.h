#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeColor
	{
	public:
		enum Predefined { WHITE, LIGHT_GRAY, GRAY, DARK_GRAY, BLACK, RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN };

	public:
		HeColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
		HeColor(int red, int green, int blue, int alpha);
		HeColor(float red_normalized, float green_normalized, float blue_normalized, float alpha_normalized);
		HeColor(Predefined predefinedColor);

		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;

		operator glm::vec4() const { return glm::vec4(r, g, b, a); }
	};

}
