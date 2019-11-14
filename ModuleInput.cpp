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
			if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || e.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
				App->renderer->WindowResized(e.window.data1, e.window.data2);
				float asp = e.window.data1 / e.window.data2;
				
				App->camera->SetAspectRatio(asp);
				App->ui->my_log.AddLog("WIDTH: %d, HEIGHT: %d", e.window.data1, e.window.data2);
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (e.button.button == SDL_BUTTON_RIGHT) {
				//If the camera mode is not Orbit
				if (App->camera->mode)
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

				if (RadToDeg(App->camera->rotZ) >= 89)
					App->camera->rotZ = DegToRad(89);

				if (RadToDeg(App->camera->rotZ) <= -89)
					App->camera->rotZ = DegToRad(-89);


				App->camera->rotY += DegToRad(xOffset) * App->camera->sensitivity;
				App->camera->rotZ += DegToRad(yOffset) * App->camera->sensitivity;

				std::cout << RadToDeg(App->camera->rotZ) << std::endl;

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

			if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
				speed_boost = true;
				App->camera->dirty = true;
			}


			if (enable_camera_movement) {
				if (e.key.keysym.scancode == SDL_SCANCODE_W) {
					if (speed_boost)
						App->camera->camSpeed.z = -3 * App->camera->speedValue;

					else
						App->camera->camSpeed.z = -App->camera->speedValue;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_S) {
					if (speed_boost)
						App->camera->camSpeed.z = 3 * App->camera->speedValue;

					else

						App->camera->camSpeed.z = App->camera->speedValue;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_D) {
					if (speed_boost)
						App->camera->camSpeed.z = 3 * App->camera->speedValue;

					else
						App->camera->camSpeed.x = App->camera->speedValue;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_A) {
					if (speed_boost)
						App->camera->camSpeed.z = -3 * App->camera->speedValue;

					else
						App->camera->camSpeed.x = -App->camera->speedValue;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_Q) {
					if (speed_boost)
						App->camera->camSpeed.z = 3 * App->camera->speedValue;

					else
						App->camera->camSpeed.y = App->camera->speedValue;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_E) {
					if (speed_boost)
						App->camera->camSpeed.z = -3 * App->camera->speedValue;

					else
						App->camera->camSpeed.y = -App->camera->speedValue;
					App->camera->dirty = true;
				}
			}

			break;

		case SDL_KEYUP:

			if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
				speed_boost = false;

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

			if (e.key.keysym.scancode == SDL_SCANCODE_Q) {

				App->camera->camSpeed.y = 0;
				App->camera->dirty = true;
			}

			if (e.key.keysym.scancode == SDL_SCANCODE_E) {

				App->camera->camSpeed.y = 0;
				App->camera->dirty = true;
			}

			break;

		case SDL_MOUSEWHEEL:
			if (e.wheel.y > 0) // scroll up
			{
				App->camera->cameraPos.z -= 1;
				App->camera->dirty = true;
			}
			else if (e.wheel.y < 0) // scroll down
			{
				App->camera->cameraPos.z += 1;
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
