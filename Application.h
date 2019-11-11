#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleProgram;
class ModuleUI;
class ModuleScene;
class ModuleModelLoader;
class ModuleCamera;
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
	ModuleRenderExercise* exercise = nullptr;
	ModuleProgram* program = nullptr;
	ModuleUI* ui = nullptr;
	ModuleTextures* textures = nullptr;
	ModuleScene* scene = nullptr;
	ModuleModelLoader* modelLoader = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleMSTimer* timer = nullptr;

	void RequestBrowser(char*);

private:

	std::list<Module*> modules;

};

extern Application* App;
