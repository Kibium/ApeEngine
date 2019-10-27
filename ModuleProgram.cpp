#include "Application.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "ModuleRenderExercise.h"

#include<iostream>
using namespace std;

ModuleProgram::ModuleProgram() {}

ModuleProgram::~ModuleProgram() {}

char* ModuleProgram::getShaderText(char* source) {
	char* data = nullptr;
	FILE* file = nullptr;
	fopen_s(&file, source, "rb");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		int size = ftell(file);
		rewind(file);
		data = (char*)malloc(size + 1);
		fread(data, 1, size, file);
		data[size] = 0;
		fclose(file);

	}

	else
		cout << "not found" << endl;

	

	return data;
}

void ModuleProgram::checkForErrors(GLuint shader, int success, char* infoLog) {

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

}

bool ModuleProgram::Init() {

	//Function that allows to get the content of a shader
	char* VSdata = getShaderText("../default.vs");
	char* FSdata = getShaderText("../default.fs");

	const GLchar *vdata = VSdata;
	const GLchar * fdata = FSdata;

	//Creating Vertex Shader
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vdata, NULL);
	glCompileShader(vShader);

	//Creating Fragment Shader
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fdata, NULL);
	glCompileShader(fShader);

	//Check that my shaders compile
	int  success = 0;
	char infoLog[512];

	checkForErrors(vShader, success, infoLog);
	checkForErrors(fShader, success, infoLog);


	//Create program object
	program = glCreateProgram();

	//Attach shaders
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	//Prepare to execute program
	glLinkProgram(program);

	//Actually use the program
	glUseProgram(program);

	//Check for errors during program compilation
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

	}

	return true;
	
}

//This updates the the color of the triangle
update_status ModuleProgram::Update() {

	timeValue = SDL_GetTicks();
	float greenValue = (sin(timeValue/800) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(program, "myColor");

	glUseProgram(program);
	glUniform4f(vertexColorLocation, greenValue, 0, 0, 1.0f);

	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {

	return true;
}