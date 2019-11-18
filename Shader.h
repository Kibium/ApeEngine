#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// constructor reads and builds the shader
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

#endif
