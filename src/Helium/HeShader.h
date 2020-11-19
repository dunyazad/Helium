#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeShader
	{
	public:
		HeShader(string vertexShaderFileName, string fragmentShaderFileName);
		HeShader(string vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName);
		~HeShader();

		void Use();

		GLint GetVertexAttributeLocation(const std::string& attributeName);
		GLint GetUniformLocation(const string& uniformName);

		void SetUniformFloat(const string& uniformName, float f);
		void SetUniformVec2(const string& uniformName, const glm::vec2& v);
		void SetUniformMat4(const string& uniformName, const glm::mat4& mat);

		inline GLuint GetProgram() { return shaderProgram; }

		inline void AddOnUseCallback(function<void(HeShader*)> callback) { onUseCallbacks.push_back(callback); }

	private:
		string vertexShaderFileName;
		string geometryShaderFileName;
		string fragmentShaderFileName;

		GLuint vertexShader;
		GLuint geometryShader;
		GLuint fragmentShader;
		GLuint shaderProgram;

		map<string, GLint> attributes;
		map<string, GLint> uniforms;

		vector<function<void(HeShader*)>> onUseCallbacks;
	};

}
