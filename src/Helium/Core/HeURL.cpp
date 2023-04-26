#include <Helium/Core/HeURL.h>
#include <Helium/Core/HeURL.h>

namespace ArtificialNature {

	string makeRelative(const string& path)
	{
		string p = path;

		if (p.front() == '/') {
			p = p.substr(1);
		}

		return p;
	}

	HeURL HeURL::FromRelativePath(const string& relativePath)
	{
		auto cwd = HeURL(HeSettings["Current Working Directory"].get<string>());
		
		string rp = relativePath;
		replace(rp.begin(), rp.end(), '\\', '/');
		if (rp.front() == '/') {
			rp = rp.substr(1);
		}

		return cwd + rp;
	}

	HeURL HeURL::GetCurrentPath()
	{
		return HeURL(HeSettings["Current Working Directory"].get<string>());
	}

	HeURL HeURL::GetShaderFileURL(const string& shaderFileName)
	{
		auto shaderRootDirectory = HeURL(HeSettings["Resource Root Directory"].get<string>()) + "shader/";
		return HeURL(shaderRootDirectory + shaderFileName);
	}

	HeURL HeURL::GetFontFileURL(const string& fontFileName)
	{
		auto fontRootDirectory = HeURL(HeSettings["Resource Root Directory"].get<string>()) + "fonts/";
		return HeURL(fontRootDirectory + fontFileName);
	}

	HeURL::HeURL()
		: path("")
	{
	}

	HeURL::HeURL(const HeURL& other)
		: path(other.path)
	{
	}

	HeURL::HeURL(const string& absolutePath)
		: path(absolutePath)
	{
		replace(path.begin(), path.end(), '\\', '/');
	}

	HeURL::~HeURL()
	{

	}

	HeURL HeURL::operator + (const HeURL& other)
	{
		if (path.back() == '/')
		{
			return HeURL(path + makeRelative(other.path));
		}
		else
		{
			return HeURL(path + "/" + makeRelative(other.path));
		}
	}

	HeURL& HeURL::operator += (const HeURL& other)
	{
		if (path.back() != '/')
		{
			path += "/";
		}
		path += makeRelative(other.path);
		return *this;
	}

	HeURL HeURL::operator + (const string& other)
	{
		if (path.back() == '/')
		{
			return HeURL(path + makeRelative(other));
		}
		else
		{
			return HeURL(path + "/" + makeRelative(other));
		}
	}

	HeURL& HeURL::operator += (const string& other)
	{
		if (path.back() != '/')
		{
			path += "/";
		}
		path += makeRelative(other);
		return *this;
	}

	HeURL operator + (const HeURL& a, const HeURL& b)
	{
		if (a.path.back() == '/')
		{
			return HeURL(a.path + makeRelative(b.path));
		}
		else
		{
			return HeURL(a.path + "/" + makeRelative(b.path));
		}
	}

	HeURL operator + (const HeURL& a, const string& b)
	{
		if (a.path.back() == '/')
		{
			return HeURL(a.path + makeRelative(b));
		}
		else
		{
			return HeURL(a.path + "/" + makeRelative(b));
		}
	}
}
