#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeShader : public HeObject
	{
	public:
		void Use();

		GLint GetVertexAttributeLocation(const std::string& attributeName);
		GLint GetUniformLocation(const string& uniformName);

		void SetUniformInt(const string& uniformName, int i);
		void SetUniformIntArray(const string& uniformName, const vector<int>& intArray);
		void SetUniformFloat(const string& uniformName, float f);
		void SetUniformFloatArray(const string& uniformName, const vector<float>& floatArray);
		void SetUniformFloatArray(const string& uniformName, float* floatArray, int elementCount);
		void SetUniformVec2(const string& uniformName, const glm::vec2& v);
		void SetUniformVec2Array(const string& uniformName, const vector<glm::vec2>& vec2Array);
		void SetUniformMat4(const string& uniformName, const glm::mat4& mat);
		void SetUniformMat4Array(const string& uniformName, const vector<glm::mat4>& mats);

		inline GLuint GetProgram() { return shaderProgram; }

		inline void AddOnUseCallback(function<void(HeShader*)> callback) { onUseCallbacks.push_back(callback); }

	protected:
		HeShader(const string& name, const HeURL& vertexShaderFileURL, const HeURL& fragmentShaderFileURL);
		HeShader(const string& name, const HeURL& vertexShaderFileURL, const HeURL& fragmentShaderFileURL, const HeURL& geometryShaderFileURL);
		~HeShader();

	private:
		HeURL vertexShaderFileURL;
		HeURL geometryShaderFileURL;
		HeURL fragmentShaderFileURL;

		GLuint vertexShader;
		GLuint geometryShader;
		GLuint fragmentShader;
		GLuint shaderProgram;

		map<string, GLint> attributes;
		map<string, GLint> uniforms;

		vector<function<void(HeShader*)>> onUseCallbacks;

	public:
		friend class HeGraphics;
	};

}
