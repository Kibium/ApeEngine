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

	GLuint program, vShader, fShader;
	float timeValue = 0;
};