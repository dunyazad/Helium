#include "HeShader.h"

namespace ArtificialNature {

	HeShader::HeShader(string vertexShaderFileName, string fragmentShaderFileName)
		: vertexShaderFileName(vertexShaderFileName), fragmentShaderFileName(fragmentShaderFileName)
	{
		{
			FILE* fp;
			fopen_s(&fp, vertexShaderFileName.c_str(), "r");
			fseek(fp, 0, SEEK_END);
			auto length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			memset(buffer, 0, length + 1);
			fread_s(buffer, length, sizeof(char), length, fp);

			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &buffer, nullptr);
			glCompileShader(vertexShader);

			delete buffer;

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		{
			FILE* fp;
			fopen_s(&fp, fragmentShaderFileName.c_str(), "r");
			fseek(fp, 0, SEEK_END);
			auto length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			memset(buffer, 0, length + 1);
			fread_s(buffer, length, sizeof(char), length, fp);

			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &buffer, nullptr);
			glCompileShader(fragmentShader);

			delete buffer;

			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	HeShader::HeShader(string vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName)
		: vertexShaderFileName(vertexShaderFileName), geometryShaderFileName(geometryShaderFileName), fragmentShaderFileName(fragmentShaderFileName)
	{
		{
			FILE* fp;
			fopen_s(&fp, vertexShaderFileName.c_str(), "r");
			fseek(fp, 0, SEEK_END);
			auto length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			memset(buffer, 0, length + 1);
			fread_s(buffer, length, sizeof(char), length, fp);

			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &buffer, nullptr);
			glCompileShader(vertexShader);

			delete buffer;

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		{
			FILE* fp;
			fopen_s(&fp, geometryShaderFileName.c_str(), "r");
			fseek(fp, 0, SEEK_END);
			auto length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			memset(buffer, 0, length + 1);
			fread_s(buffer, length, sizeof(char), length, fp);

			geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShader, 1, &buffer, nullptr);
			glCompileShader(geometryShader);

			delete buffer;

			int success;
			char infoLog[512];
			glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		{
			FILE* fp;
			fopen_s(&fp, fragmentShaderFileName.c_str(), "r");
			fseek(fp, 0, SEEK_END);
			auto length = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			char* buffer = new char[length + 1];
			memset(buffer, 0, length + 1);
			fread_s(buffer, length, sizeof(char), length, fp);

			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &buffer, nullptr);
			glCompileShader(fragmentShader);

			delete buffer;

			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	HeShader::~HeShader()
	{
	}

	void HeShader::Use()
	{
		glUseProgram(shaderProgram);

		CheckGLError();

		for (auto& callback : onUseCallbacks)
		{
			callback(this);
		}
	}

	GLint HeShader::GetVertexAttributeLocation(const std::string& attributeName)
	{
		auto i = attributes.find(attributeName);
		if (i != attributes.end())
		{
			return (*i).second;
		}

		int location = glGetAttribLocation(shaderProgram, attributeName.c_str());
		if (-1 != location)
		{
			attributes[attributeName] = location;
		}

		return location;
	}

	GLint HeShader::GetUniformLocation(const string& uniformName)
	{
		auto i = uniforms.find(uniformName);
		if (i != uniforms.end())
		{
			return (*i).second;
		}

		int location = glGetUniformLocation(shaderProgram, uniformName.c_str());
		if (-1 != location)
		{
			uniforms[uniformName] = location;
		}

		return location;
	}

	void HeShader::SetUniformFloat(const string& uniformName, float f)
	{
		glUniform1f(GetUniformLocation(uniformName), f);

		CheckGLError();
	}

	void HeShader::SetUniformVec2(const string& uniformName, const glm::vec2& v)
	{
		glUniform2fv(GetUniformLocation(uniformName), 1, (const float*)glm::value_ptr(v));

		CheckGLError();
	}

	void HeShader::SetUniformMat4(const string& uniformName, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, (const float*)glm::value_ptr(mat));

		CheckGLError();
	}
}