#include "Application.h"
#include "ModuleProgram.h"
#include <gl/glew.h>

using namespace std;

ModuleProgram::ModuleProgram() {}

ModuleProgram::~ModuleProgram() {}

char* ModuleProgram::getShaderText(char* source) {
	char* data = nullptr;
	FILE* file = nullptr;

	fopen_s(&file, source, "rb");

	if (file) {

		fseek(file, 0, SEEK_END);
		int size = ftell(file); rewind(file);
		char* data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);


	}

	return data;
}

bool ModuleProgram::Init() {

	glCreateProgram();

	//Function that allows to get the content of a shader
	char* data = getShaderText("../default.vs.txt");

	const GLchar *vdata = data;

	GLuint vShader;

	vShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(vShader, 1, &vdata, NULL);
	//glCompileShader(vShader);


	/*int  success;
	char infoLog[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}*/

	return true;
	
}

update_status ModuleProgram::Update() {

	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {

	return true;
}