#pragma once
#include<string>

void fatalError(std::string errorMessage);

void checkCompileError(unsigned int shaderId, int shaderType);

void checkLinkError(const unsigned int programId);