#pragma once
#include "Module.h"
#include "Globals.h"
#include <gl/glew.h>

class ModuleProgram : public Module {
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* getShaderText(char* source);
	void checkForErrors(GLuint a, int b, char* c);
	void InitShader(GLuint& program, GLuint& VS, GLuint& FS, char* Vdata, char* Fdata, bool actve);

	//Image
	GLuint programImage, imageVShader, imageFShader;

	//red Color
	GLuint programRed, redVShader, redFShader;

	float timeValue = 0;

};