#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeURL {
	public:
		static HeURL FromRelativePath(const string& relativePath);
		static HeURL GetCurrentPath();
		static HeURL GetShaderFileURL(const string& shaderFileName);
		static HeURL GetFontFileURL(const string& fontFileName);

	public:
		HeURL();
		HeURL(const string& absolutePath);
		HeURL(const HeURL& other);
		~HeURL();

		HeURL operator + (const HeURL& other);
		HeURL& operator += (const HeURL& other);

		HeURL operator + (const string& other);
		HeURL& operator += (const string& other);

		string path;
	};

	HeURL operator + (const HeURL& a, const HeURL& b);
	HeURL operator + (const HeURL& a, const string& b);
}