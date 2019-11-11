#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"

#include "IMGUI/imgui.h"

#include"IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_sdl.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{

	//LOG("Init SDL input event system");
	App->ui->my_log.AddLog("Init SDL input event system\n");

	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{

	static SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		// Esc button is pressed
		switch (e.type)
		{
		case SDL_QUIT:
			return UPDATE_STOP;
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				App->renderer->WindowResized(e.window.data1, e.window.data2);
				App->ui->my_log.AddLog("WIDTH: %d, HEIGHT: %d", e.window.data1, e.window.data2);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_RIGHT)
				enable_camera_movement = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_RIGHT)
				enable_camera_movement = false;

		case SDL_KEYDOWN:
			if (e.key.keysym.scancode == SDL_SCANCODE_W  && enable_camera_movement) {
				App->camera->cameraPos.z -= 0.5;				
				App->camera->dirty = true;
			}


			if (e.key.keysym.scancode == SDL_SCANCODE_S && enable_camera_movement) {
				App->camera->cameraPos.z += 0.5;
				App->camera->dirty = true;

			}

			if (e.key.keysym.scancode == SDL_SCANCODE_D && enable_camera_movement) {
				App->camera->cameraPos.x += 0.5;
				App->camera->dirty = true;
			}


			if (e.key.keysym.scancode == SDL_SCANCODE_A && enable_camera_movement) {
				App->camera->cameraPos.x -= 0.5;
				App->camera->dirty = true;

			}

			break;
		}

		ImGui_ImplSDL2_ProcessEvent(&e);

	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
