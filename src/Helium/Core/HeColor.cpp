#include <Helium/Core/HeColor.h>

namespace ArtificialNature {

	//vec3 RGB2YUV(vec3 rgb)
	//{
	//	float y = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
	//	float u = (rgb.b - y) * 0.565;
	//	float v = (rgb.r - y) * 0.713;
	//	return vec3(y, u, v);
	//}

	//vec3 YUV2RGB(vec3 yuv)
	//{
	//	float r = yuv.x + 1.403 * yuv.z;
	//	float g = yuv.x - 0.344 * yuv.y - 0.714 * yuv.z;
	//	float b = yuv.x + 1.770 * yuv.y;
	//	return vec3(r, g, b);
	//}

	//vec3 rgb2xyz(vec3 c) {
	//	vec3 tmp;
	//	tmp.x = (c.r > 0.04045) ? pow((c.r + 0.055) / 1.055, 2.4) : c.r / 12.92;
	//	tmp.y = (c.g > 0.04045) ? pow((c.g + 0.055) / 1.055, 2.4) : c.g / 12.92,
	//		tmp.z = (c.b > 0.04045) ? pow((c.b + 0.055) / 1.055, 2.4) : c.b / 12.92;
	//	const mat3 mat = mat3(
	//		0.4124, 0.3576, 0.1805,
	//		0.2126, 0.7152, 0.0722,
	//		0.0193, 0.1192, 0.9505
	//	);
	//	return mat * tmp * 100.0;
	//}

	//vec3 xyz2lab(vec3 c) {
	//	vec3 n = c / vec3(95.047, 100, 108.883);
	//	vec3 v;
	//	v.x = (n.x > 0.008856) ? pow(n.x, 1.0 / 3.0) : (7.787 * n.x) + (16.0 / 116.0);
	//	v.y = (n.y > 0.008856) ? pow(n.y, 1.0 / 3.0) : (7.787 * n.y) + (16.0 / 116.0);
	//	v.z = (n.z > 0.008856) ? pow(n.z, 1.0 / 3.0) : (7.787 * n.z) + (16.0 / 116.0);
	//	return vec3((116.0 * v.y) - 16.0, 500.0 * (v.x - v.y), 200.0 * (v.y - v.z));
	//}

	//vec3 rgb2lab(vec3 c) {
	//	vec3 lab = xyz2lab(rgb2xyz(c));
	//	return vec3(lab.x / 100.0, 0.5 + 0.5 * (lab.y / 127.0), 0.5 + 0.5 * (lab.z / 127.0));
	//}

	//vec3 lab2xyz(vec3 c) {
	//	float fy = (c.x + 16.0) / 116.0;
	//	float fx = c.y / 500.0 + fy;
	//	float fz = fy - c.z / 200.0;
	//	return vec3(
	//		95.047 * ((fx > 0.206897) ? fx * fx * fx : (fx - 16.0 / 116.0) / 7.787),
	//		100.000 * ((fy > 0.206897) ? fy * fy * fy : (fy - 16.0 / 116.0) / 7.787),
	//		108.883 * ((fz > 0.206897) ? fz * fz * fz : (fz - 16.0 / 116.0) / 7.787)
	//	);
	//}

	//vec3 xyz2rgb(vec3 c) {
	//	const mat3 mat = mat3(
	//		3.2406, -1.5372, -0.4986,
	//		-0.9689, 1.8758, 0.0415,
	//		0.0557, -0.2040, 1.0570
	//	);
	//	vec3 v = mat * (c / 100.0);
	//	vec3 r;
	//	r.x = (v.r > 0.0031308) ? ((1.055 * pow(v.r, (1.0 / 2.4))) - 0.055) : 12.92 * v.r;
	//	r.y = (v.g > 0.0031308) ? ((1.055 * pow(v.g, (1.0 / 2.4))) - 0.055) : 12.92 * v.g;
	//	r.z = (v.b > 0.0031308) ? ((1.055 * pow(v.b, (1.0 / 2.4))) - 0.055) : 12.92 * v.b;
	//	return r;
	//}

	//vec3 lab2rgb(vec3 c) {
	//	return xyz2rgb(lab2xyz(vec3(100.0 * c.x, 2.0 * 127.0 * (c.y - 0.5), 2.0 * 127.0 * (c.z - 0.5))));
	//}

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
