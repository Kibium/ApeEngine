#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"

#include "IMGUI/imgui.h"



class ModuleUI : public Module {

public:
	ModuleUI();
	~ModuleUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();

	bool showtext= false;
	ImGuiIO io;
	char* buf;
	float f;

};