#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleCamera;
class ModuleProgram;
class ModuleUI;
class ModuleTextures;
class ModelLoader;
class ModuleMSTimer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleCamera*camera = nullptr;
	ModuleProgram* program = nullptr;
	ModuleUI* ui = nullptr;
	ModuleTextures* textures = nullptr;
	ModelLoader* modelLoader = nullptr;
	ModuleMSTimer* timer = nullptr;


	void RequestBrowser(char*);

private:

	std::list<Module*> modules;

};

extern Application* App;
