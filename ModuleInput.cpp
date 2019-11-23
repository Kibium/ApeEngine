#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "ModuleProgram.h"
#include "ModuleUI.h"
#include "ModelLoader.h"


#include "IMGUI/imgui.h"

#include"IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_sdl.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{

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

update_status ModuleInput::Update()
{

	static SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&e);

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
			if (e.button.button == SDL_BUTTON_RIGHT) {

				enable_camera_movement = false;

				xOffset = 0;
				yOffset = 0;

				lastX = 0;
				lastY = 0;


			}
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

				if (!altPressed) {
					App->camera->frustum.front.x = cos(DegToRad(yaw))* cos(DegToRad(pitch));
					App->camera->frustum.front.y = sin(DegToRad(pitch));
					App->camera->frustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
					App->camera->frustum.front.Normalize();
				}

				if (altPressed) {
					App->camera->manualOrbit = true;
					App->camera->rotY += xOffset;

				}

				App->ui->my_log.AddLog("%0.1f, %0.1f\n", yaw, pitch);


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

			extension = directory.substr(directory.size() - 4, directory.size());

			if (extension == ".fbx" || extension == ".obj") {

				App->ui->my_log.AddLog("[DROP] Model loded from: ");
				App->ui->my_log.AddLog(directory.c_str());
				App->ui->my_log.AddLog("\n");

				App->modelLoader->modelDir = directory.c_str();
				App->modelLoader->hasChanged = true;

				App->camera->ResetCamera(false);
			}

			else if (extension == ".png" || extension == ".jpg" || extension == ".dds") {

				App->ui->my_log.AddLog("[DROP] Image dropped from: ");
				App->ui->my_log.AddLog(directory.c_str());
				App->ui->my_log.AddLog("\n");

				App->modelLoader->previousTexture = directory.c_str();
				App->modelLoader->textureDir = directory.c_str();

				if (extension == ".png")
					App->textures->CreateTexture(IL_PNG, directory.c_str());

				if (extension == ".jpg")
					App->textures->CreateTexture(IL_JPG, directory.c_str());

				if (extension == ".dds")
					App->textures->CreateTexture(IL_DDS, directory.c_str());



				App->modelLoader->hasChanged = true;
				App->camera->ResetCamera(false);


			}

			else {
				App->ui->my_log.AddLog("[DROP] The object dropped has to be a .fbx, .obj, .png or .jpg\n");
			}

			break;

		case SDL_KEYDOWN:

			//	App->ui->my_log.AddLog("%0.1f, %0.1f, %0.1f\n", App->camera->frustum.front.x, App->camera->frustum.front.y, App->camera->frustum.front.z);

			if (e.key.keysym.scancode == SDL_SCANCODE_LSHIFT) {
				speed_boost = true;
				App->camera->dirty = true;
			}

			if (e.key.keysym.scancode == SDL_SCANCODE_LALT)
				altPressed = true;

			if (e.key.keysym.scancode == SDL_SCANCODE_F) {

				//pitch = 0;
				//yaw = -90;

				float3 target = float3(App->modelLoader->model.scene->mRootNode->mTransformation.a4, App->modelLoader->model.scene->mRootNode->mTransformation.b4, App->modelLoader->model.scene->mRootNode->mTransformation.a4 - 1);
				
				App->camera->Focus(target, App->modelLoader->model.GetHeight());
				//float angleNew= App->camera->frustum.front.AngleBetween(target);

				//pitch = RadToDeg(angleNew);
				//App->camera->frustum.front.y = sin(DegToRad(pitch));

				//App->camera->frustum.front.x = cos(DegToRad(yaw))* cos(DegToRad(pitch));
				/*App->camera->frustum.front.y = sin(DegToRad(pitch));
				App->camera->frustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
				App->camera->frustum.front.Normalize();

				float newYaw = App->camera->frustum.front.AngleBetween(target);
				yaw = RadToDeg(newYaw);

				App->camera->frustum.front.x = cos(DegToRad(yaw))* cos(DegToRad(pitch));
				App->camera->frustum.front.y = sin(DegToRad(pitch));
				App->camera->frustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
				App->camera->frustum.front.Normalize();

		

				//App->camera->Focus(target, App->modelLoader->model.GetHeight());*/
				//App->ui->my_log.AddLog("Angle: %0.2f\n ", RadToDeg(angleNew));

				//App->camera->ResetCamera(false);
				App->camera->dirty = true;


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
