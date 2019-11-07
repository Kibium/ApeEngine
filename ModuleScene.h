#pragma once
#include "Application.h"
#include "Module.h"

#include <gl/glew.h>


class ModuleScene : public Module{
public:
	ModuleScene();
	~ModuleScene();

	bool Init();
	update_status Update();
	bool CleanUp();

};