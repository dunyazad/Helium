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

	HeURL::HeURL(const HeURL& other)
		: path(other.path)
	{
	}

	HeURL::HeURL(const string& absolutePath)
		: path(absolutePath)
	{
		replace(path.begin(), path.end(), '\\', '/');

		if (path.back() != '/') {
			path.push_back('/');
		}
	}

	HeURL::~HeURL()
	{

	}

	HeURL HeURL::operator + (const HeURL& other)
	{
		return HeURL(path + makeRelative(other.path));
	}

	HeURL& HeURL::operator += (const HeURL& other)
	{
		path += makeRelative(other.path);
		return *this;
	}

	HeURL HeURL::operator + (const string& other)
	{
		return HeURL(path + makeRelative(other));
	}

	HeURL& HeURL::operator += (const string& other)
	{
		path += makeRelative(other);
		return *this;
	}

	HeURL operator + (const HeURL& a, const HeURL& b)
	{
		return HeURL(a.path + makeRelative(b.path));
	}

	HeURL operator + (const HeURL& a, const string& b)
	{
		return HeURL(a.path + makeRelative(b));
	}
}
