#include "Application.h"
#include "ModuleProgram.h"
#include "SDL.h"
#include "ModuleCamera.h"
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

void ModuleProgram::InitShader(GLuint& program, GLuint& VS, GLuint& FS, char* Vdata, char* Fdata, bool active) {

	const GLchar *vdata = Vdata;
	const GLchar *fdata = Fdata;

	//Creating Vertex Shader
	VS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VS, 1, &vdata, NULL);
	glCompileShader(VS);

	//Creating Fragment Shader
	FS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FS, 1, &fdata, NULL);
	glCompileShader(FS);

	//Check that my shaders compile
	int  success = 0;
	char infoLog[512];

	checkForErrors(VS, success, infoLog);
	checkForErrors(FS, success, infoLog);


	//Create program object
	program = glCreateProgram();

	//Attach shaders
	glAttachShader(program, VS);
	glAttachShader(program, FS);

	//Prepare to execute program
	glLinkProgram(program);

	//Actually use the program
	if(active)
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


}

bool ModuleProgram::Init() {

	App->ui->my_log.AddLog("Initializing Module Program\n");

	//Function that allows to get the content of a shader
	char* imageVSdata = getShaderText("../quad.vs");
	char* imageFSdata = getShaderText("../quad.fs");

	char* redVSdata = getShaderText("../red.vs");
	char* redFSdata = getShaderText("../red.fs");

	InitShader(programImage, imageVShader, imageFShader, imageVSdata, imageFSdata, true);
	InitShader(programRed, redVShader, redFShader, redVSdata, redFSdata, false);


	/*const GLchar *vdata = VSdata;
	const GLchar *fdata = FSdata;

	//Creating Vertex Shader
	imageVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(imageVShader, 1, &vdata, NULL);
	glCompileShader(imageVShader);

	//Creating Fragment Shader
	imageFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(imageFShader, 1, &fdata, NULL);
	glCompileShader(imageFShader);

	//Check that my shaders compile
	int  success = 0;
	char infoLog[512];

	checkForErrors(imageVShader, success, infoLog);
	checkForErrors(imageFShader, success, infoLog);


	//Create program object
	programImage = glCreateProgram();

	//Attach shaders
	glAttachShader(programImage, imageVShader);
	glAttachShader(programImage, imageFShader);

	//Prepare to execute program
	glLinkProgram(programImage);

	//Actually use the program
	glUseProgram(programImage);

	//Check for errors during program compilation
	glGetProgramiv(programImage, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programImage, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;

	}
	else
		glGetProgramInfoLog(programImage, 512, NULL, infoLog);

*/
	//Transform matrix to the shader!
	int location = glGetUniformLocation(programImage, "transform");
	glUniformMatrix4fv(location, 1, GL_TRUE, &App->camera->transform[0][0]); //Calculating vertexs in the vertex shader

	return true;
	
}

//This updates the the color of the triangle
update_status ModuleProgram::Update() {

	/*timeValue = SDL_GetTicks();
	float greenValue = (sin(timeValue/800) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(programImage, "myColor");

	glUseProgram(programImage);
	glUniform4f(vertexColorLocation, greenValue, 0, 0, 1.0f);*/
	

	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {


	//clean glVertexBuffer, VBO, EBO, etc
	// delting every new
	return true;
}