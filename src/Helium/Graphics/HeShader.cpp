#include <Helium/Graphics/HeShader.h>

namespace ArtificialNature {

	HeShader::HeShader(const string& name, const string& vertexShaderFileName, string fragmentShaderFileName)
		: HeObject(name), vertexShaderFileName(vertexShaderFileName), fragmentShaderFileName(fragmentShaderFileName)
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




		//GLint count;
		//glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &count);
		//printf("Active Uniforms: %d\n", count);

		//GLint size; // size of the variable
		//GLenum type; // type of the variable (float, vec3 or mat4, etc)
		//const GLsizei bufSize = 16; // maximum name length
		//GLchar tname[bufSize]; // variable name in GLSL
		//GLsizei length;

		//for (int i = 0; i < count; i++)
		//{
		//	glGetActiveUniform(shaderProgram, (GLuint)i, bufSize, &length, &size, &type, tname);

		//	printf("Uniform #%d Type: %u Name: %s\n", i, type, tname);
		//}





		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	HeShader::HeShader(const string& name, const string& vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName)
		: HeObject(name), vertexShaderFileName(vertexShaderFileName), geometryShaderFileName(geometryShaderFileName), fragmentShaderFileName(fragmentShaderFileName)
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

	void HeShader::SetUniformInt(const string& uniformName, int i)
	{
		glUniform1i(GetUniformLocation(uniformName), i);

		CheckGLError();
	}

	void HeShader::SetUniformIntArray(const string& uniformName, const vector<int>& intArray)
	{
		glUniform1iv(GetUniformLocation(uniformName), intArray.size(), &intArray[0]);

		CheckGLError();
	}

	void HeShader::SetUniformFloat(const string& uniformName, float f)
	{
		glUniform1f(GetUniformLocation(uniformName), f);

		CheckGLError();
	}

	void HeShader::SetUniformFloatArray(const string& uniformName, const vector<float>& floatArray)
	{
		glUniform1fv(GetUniformLocation(uniformName), floatArray.size(), &floatArray[0]);

		CheckGLError();
	}

	void HeShader::SetUniformFloatArray(const string& uniformName, float* floatArray, int elementCount)
	{
		glUniform1fv(GetUniformLocation(uniformName), elementCount, floatArray);

		CheckGLError();
	}

	void HeShader::SetUniformVec2(const string& uniformName, const glm::vec2& v)
	{
		glUniform2fv(GetUniformLocation(uniformName), 1, (const float*)glm::value_ptr(v));

		CheckGLError();
	}

	void HeShader::SetUniformVec2Array(const string& uniformName, const vector<glm::vec2>& vec2Array)
	{
		glUniform2fv(GetUniformLocation(uniformName), vec2Array.size(), (const float*)&vec2Array[0]);

		CheckGLError();
	}

	void HeShader::SetUniformMat4(const string& uniformName, const glm::mat4& mat)
	{
		glUniformMatrix4fv(GetUniformLocation(uniformName), 1, false, (const float*)glm::value_ptr(mat));

		CheckGLError();
	}

	void HeShader::SetUniformMat4Array(const string& uniformName, const vector<glm::mat4>& mats)
	{
		glUniformMatrix4fv(GetUniformLocation(uniformName), mats.size(), false, (const float*)&(mats[0]));

		CheckGLError();
	}
}