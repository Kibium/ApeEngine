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

const void ModuleProgram::checkForErrors(GLuint& shader, int success, char* infoLog) {

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

void ModuleProgram::Use(const GLuint& program) {
	glUseProgram(program);
}

//ORBIT
void ModuleProgram::updateProgram(const GLuint& program, float4x4& m, float4x4& v, float4x4& p) {
	Use(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &m[0][0]); //Calculating vertexs in the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &v[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &p[0][0]);
}

//NO ORBIT
void ModuleProgram::updateProgram(const GLuint& program, float4x4& v, float4x4& p) {
	Use(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &v[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, &p[0][0]);
}

bool ModuleProgram::Init() {

	App->ui->my_log.AddLog("Init Program Module\n");

	char* defVdata = getShaderText("../Source/Shaders/default.vs");
	char* defFdata = getShaderText("../Source/Shaders/default.fs");

	char* linesVdata = getShaderText("../Source/Shaders/lines.vs");
	char* linesFdata = getShaderText("../Source/Shaders/lines.fs");

	char* phongVdata = getShaderText("../Source/Shaders/Phong.vs");
	char*phongFdata = getShaderText("../Source/Shaders/Phong.fs");

	//Initialize
	InitShader(defaultProgram, defVS, defFS, defVdata, defFdata);
	InitShader(linesProgram, linesVS, linesFS, linesVdata, linesFdata);
	InitShader(phongProgram, phongVS, phongFS, phongVdata, phongFdata);
	
	rotx = roty = rotz = 0;
	rotateM = float3x3::RotateX(rotx) * float3x3::RotateY(roty) * float3x3::RotateZ(rotz);
	model = float4x4::FromTRS(float3(0, 3, 0), rotateM, float3(1, 1, 1));

	//Setting values to our shaders
	updateProgram(defaultProgram, model, App->camera->view, App->camera->proj);	
	updateProgram(linesProgram, model, App->camera->view, App->camera->proj);
	updateProgram(phongProgram, model, App->camera->view, App->camera->proj);


	return true;

}


//This updates the the color of the triangle
update_status ModuleProgram::Update() {

	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp() {


	//clean glVertexBuffer, VBO, EBO, etc
	// delting every new
	return true;
}