#pragma once
#include "Module.h"
#include "Globals.h"

#include "IMGUI/imgui.h"
#include"IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_sdl.h"

class ModuleUI : public Module {

public:
	ModuleUI();
	~ModuleUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	bool CleanUp();

	bool showWindow = true;

	ImGuiIO& io = ImGui::GetIO();
};