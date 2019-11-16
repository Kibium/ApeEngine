#pragma once
#include "Application.h"
#include "Module.h"
#include "Shader.h"
#include "Model.h"

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

private:
	Shader modelShader;
	Model baker;

};