#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include "ModuleModelLoader.h"
#include "SDL.h"

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

				float asp = RadToDeg(App->window->GetWidth() / App->window->GetHeight());

				App->camera->SetAspectRatio(asp);

				App->camera->dirty = true;
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
				currentX = (float)e.motion.x;
				currentY = (float)e.motion.y;

				if (!once) {
					lastX = currentX;
					lastY = currentY;
					once = true;
				}

				xOffset = currentX - lastX;
				yOffset = lastY - currentY;

				lastX = currentX;
				lastY = currentY;

				xOffset *= App->camera->sensitivity;
				yOffset *= App->camera->sensitivity;

				yaw += xOffset;
				pitch += yOffset;

				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;


				//App->camera->rotY += DegToRad(xOffset) * App->camera->sensitivity;
				//App->camera->rotZ += DegToRad(yOffset) * App->camera->sensitivity;

				App->camera->frustum.front.x = cos(DegToRad(yaw))* cos(DegToRad(pitch));
				App->camera->frustum.front.y = sin(DegToRad(pitch));
				App->camera->frustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
				App->camera->frustum.front.Normalize();

				//App->camera->frustum.up.x *= cos(DegToRad(yaw))* cos(DegToRad(pitch));
				//App->camera->frustum.up.y *= sin(DegToRad(pitch));
				//App->camera->frustum.up.z *= sin(DegToRad(yaw)) * cos(DegToRad(pitch));
				//App->camera->frustum.up.Normalize();

				App->camera->dirty = true;
			}

			else {

				lastX = 0;
				lastY = 0;
			}
			break;

		case SDL_DROPFILE:
			modelOnce = false;
			directory = e.drop.file;


			//App->ui->my_log.AddLog(directory.substr(directory.size() - 4, directory.size()).c_str());

			if (directory.substr(directory.size() - 4, directory.size()) == ".fbx" || directory.substr(directory.size() - 4, directory.size()) == ".obj") {
				App->ui->my_log.AddLog("Model loded from: ");
				App->ui->my_log.AddLog(directory.c_str());
				App->ui->my_log.AddLog("\n");
				App->modelLoader->dir = directory.c_str();
				App->modelLoader->hasChanged = true;
			}

			if (directory.substr(directory.size() - 4, directory.size()) == ".png") {
				App->ui->my_log.AddLog("PNG Dropped");
				//Drop texture
				//aiMaterial* material = App->modelLoader->model->scene->mMaterials[mesh->mMaterialIndex];
			}





			break;

		case SDL_KEYDOWN:

			if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
				speed_boost = true;
				App->camera->dirty = true;
			}

			if (e.key.keysym.scancode == SDL_SCANCODE_LALT)
				altPressed = true;

			if (e.key.keysym.scancode == SDL_SCANCODE_F) {

				yaw = 0;
				pitch = 0;
				App->camera->ResetCamera();

			}

			if (enable_camera_movement) {
				if (e.key.keysym.scancode == SDL_SCANCODE_W) {
					if (speed_boost)
						App->camera->camSpeed = 3 * App->camera->speedValue * App->camera->frustum.front;

					else
						App->camera->camSpeed = App->camera->speedValue * App->camera->frustum.front;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_S) {
					if (speed_boost)
						App->camera->camSpeed = -3 * App->camera->speedValue * App->camera->frustum.front;

					else

						App->camera->camSpeed = -App->camera->speedValue  * App->camera->frustum.front;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_D) {
					if (speed_boost)
						App->camera->camSpeed = 3 * App->camera->speedValue  * App->camera->camRight;

					else
						App->camera->camSpeed = App->camera->speedValue * App->camera->camRight;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_A) {
					if (speed_boost)
						App->camera->camSpeed = -3 * App->camera->speedValue* App->camera->camRight;

					else
						App->camera->camSpeed = -App->camera->speedValue* App->camera->camRight;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_Q) {
					if (speed_boost)
						App->camera->camSpeed = 3 * App->camera->speedValue * App->camera->frustum.up;

					else
						App->camera->camSpeed = App->camera->speedValue * App->camera->frustum.up;
					App->camera->dirty = true;
				}

				if (e.key.keysym.scancode == SDL_SCANCODE_E) {
					if (speed_boost)
						App->camera->camSpeed = -3 * App->camera->speedValue * App->camera->frustum.up;

					else
						App->camera->camSpeed = -App->camera->speedValue * App->camera->frustum.up;
					App->camera->dirty = true;
				}
			}

			break;

		case SDL_KEYUP:
			if (e.key.keysym.scancode == SDL_SCANCODE_F)
				App->camera->dirty = true;

			if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT)
				speed_boost = false;

			if (e.key.keysym.scancode == SDL_SCANCODE_LALT)
				altPressed = true;

			if (e.key.keysym.scancode == SDL_SCANCODE_W || e.key.keysym.scancode == SDL_SCANCODE_S ||
				e.key.keysym.scancode == SDL_SCANCODE_D || e.key.keysym.scancode == SDL_SCANCODE_A ||
				e.key.keysym.scancode == SDL_SCANCODE_Q || e.key.keysym.scancode == SDL_SCANCODE_E) {
				App->camera->camSpeed = float3::zero;
				App->camera->dirty = true;
			}



			break;

		case SDL_MOUSEWHEEL:
			if (e.wheel.y > 0) // scroll up
			{
				App->camera->frustum.pos.z -= 1;
				App->camera->dirty = true;
			}
			else if (e.wheel.y < 0) // scroll down
			{
				App->camera->frustum.pos.z += 1;
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
