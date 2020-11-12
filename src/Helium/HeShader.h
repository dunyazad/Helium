#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeShader
	{
	public:
		HeShader(string vertexShaderFileName, string fragmentShaderFileName);
		~HeShader();

		void Use();

		unsigned int GetVertexAttributeLocation(const std::string& attributeName);

		inline GLuint GetProgram() { return shaderProgram; }

	private:
		string vertexShaderFileName;
		string fragmentShaderFileName;

		string vertexShaderSource;
		string fragmentShaderSource;

		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;

		map<string, GLint> attributes;
	};

}
