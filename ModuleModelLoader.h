#pragma once
#include "Application.h"
#include "Module.h"

#include <vector>

class ModuleModelLoader: public Module {
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status Update();
	bool CleanUp();
	void loadModel();

	int numMeshes = 0;

};