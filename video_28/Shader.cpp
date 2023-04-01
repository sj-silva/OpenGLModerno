#include<gl/glew.h>
#include<fstream>
#include<sstream>
#include<glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Error.h"

Shader::Shader(const std::string vertFilePath, const std::string fragFilePath):
	_programId(0),
	_vertexShaderId(0),
	_fragmentShaderId(0),
	_vertFilePath(vertFilePath),
	_fragFilePath(fragFilePath)
{
}


Shader::~Shader()
{
	glDeleteProgram(_programId);
}

void Shader::Bind() const
{
	glUseProgram(_programId);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SendUniformData(const std::string& variableName, const glm::mat4& data)
{
	int Id = GetUniformLocation(variableName);
	glUniformMatrix4fv(Id, 1, GL_FALSE, glm::value_ptr(data));
}

void Shader::CreateShaders()
{
	_programId = glCreateProgram();
	if (_programId == 0)
	{
		fatalError("Error creating Program Object.");
	}

	_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderId == 0) 
	{
		fatalError("Error creating Vertex Shader object.");
	}

	_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderId == 0)
	{
		fatalError("Error creating Fragment Shader object.");
	}

	CompileShaders();
	LinkProgram();
}

void Shader::CompileShaders()
{
	
	std::string vsCode, fsCode;
	vsCode = ReadProgramSource(_vertFilePath);
	fsCode = ReadProgramSource(_fragFilePath);

	const char* vsFinalCode = vsCode.c_str();
	const char* fsFinalCode = fsCode.c_str();

	
	glShaderSource(_vertexShaderId, 1, &vsFinalCode, NULL);
	glShaderSource(_fragmentShaderId, 1, &fsFinalCode, NULL);

	
	glCompileShader(_vertexShaderId);
	checkCompileError(_vertexShaderId, GL_VERTEX_SHADER);

	glCompileShader(_fragmentShaderId);
	checkCompileError(_fragmentShaderId, GL_FRAGMENT_SHADER);
}

void Shader::LinkProgram()
{
	glAttachShader(_programId, _vertexShaderId);
	glAttachShader(_programId, _fragmentShaderId);
	{
		glLinkProgram(_programId);
		checkLinkError(_programId);
	}
	glDetachShader(_programId, _vertexShaderId);
	glDetachShader(_programId, _fragmentShaderId);

	glDeleteShader(_vertexShaderId);
	glDeleteShader(_fragmentShaderId);
}

int Shader::GetUniformLocation(const std::string& variableName)
{
	if (_uniformLocation.find(variableName) != _uniformLocation.end())
	{
		return _uniformLocation[variableName];
	}
	
	int Id= glGetUniformLocation(_programId, variableName.c_str());
	if (Id == -1) 
	{
		fatalError("Uniform  " + variableName + " not found");
	}
	_uniformLocation[variableName] = Id;

	return Id;
}

std::string Shader::ReadProgramSource(const std::string filePath)
{
	std::fstream file(filePath);

	std::stringstream ss;
	std::string line;

	while (getline(file, line))
	{
		ss << line << "\n";
	}
	file.close();

	return ss.str();
}
