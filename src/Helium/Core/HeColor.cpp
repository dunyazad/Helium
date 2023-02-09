#include <Helium/Core/HeColor.h>

namespace ArtificialNature {

	HeColor::HeColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
	{
		r = red / 255.0f;
		g = green / 255.0f;
		b = blue / 255.0f;
		a = alpha / 255.0f;
	}

	HeColor::HeColor(int red, int green, int blue, int alpha)
	{
		r = red / 255.0f;
		g = green / 255.0f;
		b = blue / 255.0f;
		a = alpha / 255.0f;
	}

	HeColor::HeColor(float red_normalized, float green_normalized, float blue_normalized, float alpha_normalized)
	{
		r = red_normalized;
		g = green_normalized;
		b = blue_normalized;
		a = alpha_normalized;
	}

	HeColor::HeColor(Predefined predefinedColor)
	{
		switch (predefinedColor)
		{
		case ArtificialNature::HeColor::WHITE:
			r = 1.0f; g = 1.0f; b = 1.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::LIGHT_GRAY:
			r = 0.75f; g = 0.75f; b = 0.75f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::GRAY:
			r = 0.5f; g = 0.5f; b = 0.5f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::DARK_GRAY:
			r = 0.25f; g = 0.25f; b = 0.25f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::BLACK:
			r = 0.0f; g = 0.0f; b = 0.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::RED:
			r = 1.0f; g = 0.0f; b = 0.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::GREEN:
			r = 0.0f; g = 1.0f; b = 0.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::BLUE:
			r = 0.0f; g = 0.0f; b = 1.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::YELLOW:
			r = 1.0f; g = 1.0f; b = 0.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::MAGENTA:
			r = 1.0f; g = 0.0f; b = 1.0f; a = 1.0f;
			break;
		case ArtificialNature::HeColor::CYAN:
			r = 0.0f; g = 1.0f; b = 1.0f; a = 1.0f;
			break;
		default:
			break;
		}
	}
}
