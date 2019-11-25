#ifndef __ModuleLoader_H__
#define __ModuleLoader_H__
#include "Application.h"
#include "Module.h"
#include "Shader.h"
#include "Model.h"

#include <vector>

class ModelLoader : public Module {
public:
	ModelLoader();
	~ModelLoader();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void loadModel();

	int numMeshes = 0;
	Model model;
	Shader modelShader;

	std::string modelDir;
	std::string textureDir;
	std::string previousTexture;
	std::string checkerDir;

	bool activate_checker = false;
	bool iterated = false;
	bool hasChanged = true;
	bool loadFromAbsolutePath = false;
private:


};

#endif