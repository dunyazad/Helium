#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
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

	ostream& operator << (ostream& o, const glm::vec4& v)
	{
		return o << v.x << "\t" << v.y << "\t" << v.z << "\t" << v.w;
	}

	ostream& operator << (ostream& o, const glm::mat3& m)
	{
		return o << m[0] << endl << m[1] << endl << m[2];
	}

	ostream& operator << (ostream& o, const glm::mat4& m)
	{
		return o << m[0] << endl << m[1] << endl << m[2] << endl << m[3];
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

	unsigned int NextPowerOf2(unsigned int n)
	{
		unsigned int p = 1;
		if (n && !(n & (n - 1)))
			return n;

		while (p < n)
			p <<= 1;

		return p;
	}
}

namespace glm {
	mat4 flip_axes(const mat4& m)
	{
		mat4 rm;
		rm[0][0] = m[0][0];
		rm[0][1] = -m[0][1];
		rm[0][2] = -m[0][2];
		rm[0][3] = m[0][3];

		rm[1][0] = m[1][0];
		rm[1][1] = -m[1][1];
		rm[1][2] = -m[1][2];
		rm[1][3] = m[1][3];

		rm[2][0] = m[2][0];
		rm[2][1] = -m[2][1];
		rm[2][2] = -m[2][2];
		rm[2][3] = m[2][3];

		rm[3][0] = m[3][0];
		rm[3][1] = m[3][1];
		rm[3][2] = m[3][2];
		rm[3][3] = m[3][3];

		return rm;
	}
}