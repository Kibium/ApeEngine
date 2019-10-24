#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleProgram : public Module {
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* getShaderText(char* source);

};