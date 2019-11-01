#include "Application.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "ModuleRender.h"
#include "ModuleUI.h"

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
	else
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

	
}

bool ModuleProgram::Init() {

	App->ui->my_log.AddLog("Initializing Module Program\n");

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
	else
		glGetProgramInfoLog(program, 512, NULL, infoLog);


	//Transform matrix to the shader!
	int location = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(location, 1, GL_TRUE, &App->renderer->transform[0][0]); //Calculating vertexs in the vertex shader

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