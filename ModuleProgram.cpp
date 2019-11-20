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

void ModuleProgram::InitShader(GLuint& program, GLuint& VS, GLuint& FS, char* Vdata, char* Fdata) {

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

void ModuleProgram::Use(GLuint& program) {
	glUseProgram(program);
}

bool ModuleProgram::Init() {

	App->ui->my_log.AddLog("Init Program Module\n");


	char* defVdata = getShaderText("../default.vs");
	char* defFdata = getShaderText("../default.fs");

	char* texVdata = getShaderText("../noTexture.vs");
	char* texFdata = getShaderText("../noTexture.fs");

	char* modVdata = getShaderText("../model.vs");
	char* modFdata = getShaderText("../model.fs");

	//Initialize
	InitShader(defaultProgram, defVS, defFS, defVdata, defFdata);
	InitShader(noTexProgram, texVS, texFS, texVdata, texFdata);
	//InitShader(modelProgram, progVS, progFS, modVdata, modFdata);

	Use(defaultProgram);
	rotx = roty = rotz = 0;
	rotateM = float3x3::RotateX(rotx) * float3x3::RotateY(roty) * float3x3::RotateZ(rotz);
	model = float4x4::FromTRS(float3(0, 3, 0), rotateM, float3(1, 1, 1));

	//default shader//
	modelLocation = glGetUniformLocation(defaultProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader

	viewLocation = glGetUniformLocation(defaultProgram, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &App->camera->view[0][0]);

	projLocation = glGetUniformLocation(defaultProgram, "proj");
	glUniformMatrix4fv(projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);

	//Use(noTexProgram);
	//model = float4x4::identity;
	////grid Shader//
	//modelLocation = glGetUniformLocation(noTexProgram, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader

	//viewLocation = glGetUniformLocation(noTexProgram, "view");
	//glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &App->camera->view[0][0]);

	//projLocation = glGetUniformLocation(noTexProgram, "proj");
	//glUniformMatrix4fv(projLocation, 1, GL_TRUE, &App->camera->proj[0][0]);
	//Use(defaultProgram);


	return true;

}

//This updates the the color of the triangle
update_status ModuleProgram::Update() {

	/*roty += 0.005;
	float3x3 rotateM = float3x3::RotateX(rotx) * float3x3::RotateY(roty) * float3x3::RotateZ(rotz);
	model = float4x4::FromTRS(float3(0, 2, 0), rotateM, float3(1, 1, 1));*/
	//Use(defaultProgram);
	//modelLocation = glGetUniformLocation(defaultProgram, "model");
	//glUniformMatrix4fv(modelLocation, 1, GL_TRUE, &model[0][0]); //Calculating vertexs in the vertex shader
	//glUseProgram(0);
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {


	//clean glVertexBuffer, VBO, EBO, etc
	// delting every new
	return true;
}