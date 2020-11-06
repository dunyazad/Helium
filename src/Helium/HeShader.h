#pragma once

#include "HeliumCommon.h"

//namespace ArtificialNature {
//
//    class HeShader
//    {
//    public:
//        unsigned int ID;
//        HeShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
//
//        void use();
//        void setBool(const string& name, bool value) const;
//        void setInt(const string& name, int value) const;
//        void setFloat(const string& name, float value) const;
//        void setVec2(const string& name, const glm::vec2& value) const;
//        void setVec2(const string& name, float x, float y) const;
//        
//        void setVec3(const string& name, const glm::vec3& value) const;
//        void setVec3(const string& name, float x, float y, float z) const;
//        
//        void setVec4(const string& name, const glm::vec4& value) const;
//        void setVec4(const string& name, float x, float y, float z, float w);
//        
//        void setMat2(const string& name, const glm::mat2& mat) const;
//        void setMat3(const string& name, const glm::mat3& mat) const;
//        void setMat4(const string& name, const glm::mat4& mat) const;
//
//    private:
//        void checkCompileErrors(GLuint shader, string type);
//    };
//
//}


namespace ArtificialNature {

	class HeShader
	{
	public:
		HeShader(string vertexShaderFileName, string fragmentShaderFileName);
		~HeShader();

		void Use();

	private:
		string vertexShaderFileName;
		string fragmentShaderFileName;

		string vertexShaderSource;
		string fragmentShaderSource;

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
	};

}
