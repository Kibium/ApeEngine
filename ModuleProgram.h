#pragma once
#include "Module.h"
#include "Globals.h"
#include <gl/glew.h>

#include "MathGeoLib.h"

class ModuleProgram : public Module {
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* getShaderText(char* source);
	void checkForErrors(GLuint a, int b, char* c);
	void InitShader(GLuint& program, GLuint& VS, GLuint& FS, char* Vdata, char* Fdata);
	inline void Use(GLuint& program);
	//Default
	GLuint defaultProgram, defVS, defFS;
	GLuint noTexProgram, texVS, texFS;
	GLuint modelProgram, progVS, progFS;
	float4x4 model;
	float3x3 rotateM;

	float rotx, roty, rotz;

	//Transform matrix location
	int modelLocation, viewLocation, projLocation;


};