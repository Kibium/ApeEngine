#ifndef __ModuleProgram_H__
#define __ModuleProgram_H__

#include "Module.h"
#include "Globals.h"
#include <gl/glew.h>

#include "MathGeoLib.h"

class ModuleProgram : public Module {
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	char* getShaderText(char* source);
	const void checkForErrors(GLuint& a, int b, char* c);
	void InitShader(GLuint& program, GLuint& VS, GLuint& FS, char* Vdata, char* Fdata);

	void updateProgram(const GLuint& program, float4x4& m, float4x4& v, float4x4& p);
	void updateProgram(const GLuint& program, float4x4& v, float4x4& p);
	inline void Use(const GLuint& program);

	//Default
	GLuint defaultProgram, defVS, defFS;
	GLuint linesProgram, linesVS, linesFS;
	GLuint modelProgram, progVS, progFS;
	float4x4 model;
	float3x3 rotateM;

	float rotx, roty, rotz;

	//Transform matrix location
	int modelLocation, viewLocation, projLocation;

};

#endif