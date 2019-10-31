#pragma once
#include "Application.h"
#include "Module.h"
#include "Globals.h"
#include <GL/glew.h>

class ModuleTextures : public Module {

public:
	ModuleTextures();
	~ModuleTextures();

	bool Init();

	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();
	GLuint texture;

private:


};