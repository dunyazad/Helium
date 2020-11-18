#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeShader
	{
	public:
		HeShader(string vertexShaderFileName, string fragmentShaderFileName);
		~HeShader();

		void Use();

		GLint GetVertexAttributeLocation(const std::string& attributeName);
		GLint GetUniformLocation(const string& uniformName);

		void SetUniformMat4(const string& uniformName, const glm::mat4& mat);

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
		map<string, GLint> uniforms;
	};

}
