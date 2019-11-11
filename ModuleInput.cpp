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
			if (e.button.button == SDL_BUTTON_RIGHT) {
				enable_camera_movement = true;

				//Reset the condition so offsets are reseted once
				once = false;

			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (e.button.button == SDL_BUTTON_RIGHT) 
				enable_camera_movement = false;
				
			
				
			break;
		case SDL_MOUSEMOTION:
			if (enable_camera_movement) {

				currentX = e.motion.x;
				currentY = e.motion.y;

				xOffset = currentX - lastX;
				yOffset = currentY - lastY;

				lastX = currentX;
				lastY = currentY;

				//resets offsets when we release the mouse button
				if (!once) {
					xOffset = 0;
					yOffset = 0;
					once = true;
				}

				App->camera->rotY += xOffset/100;
				App->camera->rotZ += yOffset/100;

				App->camera->camTarget.x = cos(DegToRad(yOffset)) * cos(DegToRad(xOffset));
				App->camera->camTarget.y = sin(DegToRad(yOffset));
				App->camera->camTarget.z = cos(DegToRad(yOffset)) * sin(DegToRad(xOffset));
				App->camera->camTarget.Normalize();
				
				App->camera->dirty = true;

				//App->ui->my_log.AddLog("xOffset: %f  yOffset: %f \n", xOffset, yOffset);
			
			}

			else {
				currentX = 0;
				currentY = 0;

				xOffset = 0;
				yOffset = 0;

				lastX = 0;
				lastY = 0;
			}
			break;



		case SDL_KEYDOWN:
			if (enable_camera_movement) {
				if (e.key.keysym.scancode == SDL_SCANCODE_W) {
					App->camera->camSpeed.z -= 0.1;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_S) {
					App->camera->camSpeed.z += 0.1;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_D) {
					App->camera->camSpeed.x += 0.1;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_A) {
					App->camera->camSpeed.x -= 0.1;
					App->camera->dirty = true;
				}
			}
			
			break;

		case SDL_KEYUP:
			if (e.key.keysym.scancode == SDL_SCANCODE_W) {
				App->camera->camSpeed.z = 0;
				App->camera->dirty = true;
			}


			if (e.key.keysym.scancode == SDL_SCANCODE_S) {
				App->camera->camSpeed.z = 0;
				App->camera->dirty = true;
			}


			if (e.key.keysym.scancode == SDL_SCANCODE_D) {
				App->camera->camSpeed.x = 0;
				App->camera->dirty = true;
			}


			if (e.key.keysym.scancode == SDL_SCANCODE_A) {
				App->camera->camSpeed.x = 0;
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
