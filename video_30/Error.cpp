#include<gl/glew.h>
#include<iostream>
#include<cstdlib>

#include "Error.h"


void fatalError(std::string errorMessage)
{
	std::cout << "ERROR:: " + errorMessage + "\n";
	std::cin.get();
	exit(EXIT_FAILURE);
}

void checkCompileError(unsigned int shaderId, int shaderType)
{
    int compileStatus;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_FALSE)
    {
        int infoLogLength;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* message = new char[infoLogLength];

        glGetShaderInfoLog(shaderId, infoLogLength, NULL, message);
        std::string myShaderTxt = (shaderType == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER");
        
        std::cout << message << std::endl;
        delete[] message;

        fatalError("Compilation failed in " + myShaderTxt);
        
    }
    else {
        std::string myShaderTxt = (shaderType == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER");
        std::cout << "Shader compilation successful in " + myShaderTxt << std::endl;
    }
}

void checkLinkError(const unsigned int programId)
{
    int linkStatus;
    glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        int infoLogLength;
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* message = new char[infoLogLength];

        glGetProgramInfoLog(programId, infoLogLength, NULL, message);
        std::cout << message << std::endl;
        fatalError("Shaders failed to link!!!");

        glDeleteProgram(programId);

        delete[] message;

    }
    else {
        std::cout << "Shader link was successful! \n";
    }
}
