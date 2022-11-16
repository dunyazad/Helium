#define TINYPLY_IMPLEMENTATION
#include <Helium/Core/HeliumCommon.h>

void _CheckGLError(const char* file, int line)
{
    GLenum err(glGetError());

    while (err != GL_NO_ERROR)
    {
        std::string error;
        switch (err)
        {
        case GL_INVALID_OPERATION:  error = "INVALID_OPERATION";      break;
        case GL_INVALID_ENUM:       error = "INVALID_ENUM";           break;
        case GL_INVALID_VALUE:      error = "INVALID_VALUE";          break;
        case GL_OUT_OF_MEMORY:      error = "OUT_OF_MEMORY";          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
        }
        std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
        err = glGetError();
    }

    return;
}

namespace ArtificialNature {

	ostream& operator << (ostream& o, const glm::vec2& v)
	{
		return o << v.x << " " << v.y;
	}

	ostream& operator << (ostream& o, const glm::vec3& v)
	{
		return o << v.x << " " << v.y << " " << v.z;
	}

	int safe_stoi(const string& input)
	{
		if (input.empty())
		{
			return INT_MAX;
		}
		else
		{
			return stoi(input);
		}
	}

	float safe_stof(const string& input)
	{
		if (input.empty())
		{
			return FLT_MAX;
		}
		else
		{
			return stof(input);
		}
	}

	vector<string> split(const string& input, const string& delimiters, bool includeEmptyString)
	{
		vector<string> result;
		string piece;
		for (auto c : input)
		{
			bool contains = false;
			for (auto d : delimiters)
			{
				if (d == c)
				{
					contains = true;
					break;
				}
			}

			if (contains == false)
			{
				piece += c;
			}
			else
			{
				if (includeEmptyString || piece.empty() == false)
				{
					result.push_back(piece);
					piece.clear();
				}
			}
		}
		if (piece.empty() == false)
		{
			result.push_back(piece);
		}

		return result;
	}

}