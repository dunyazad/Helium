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
		HeShader();
		~HeShader();

		void Use();

	private:
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\0";

		unsigned int vertexShader;
		unsigned int fragmentShader;
		unsigned int shaderProgram;
	};

}