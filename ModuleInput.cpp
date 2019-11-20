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
				//SDL_SetWindowSize(App->window->window, App->window->GetWidth(), App->window->GetHeight());

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

				App->camera->frustum.front.x = cos(DegToRad(yaw))* cos(DegToRad(pitch));
				App->camera->frustum.front.y = sin(DegToRad(pitch));
				App->camera->frustum.front.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));
				App->camera->frustum.front.Normalize();

				//App->ui->my_log.AddLog("%f, %f, %f \n", App->camera->frustum.front.x, App->camera->frustum.front.y, App->camera->frustum.front.z);

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
				//App->ui->my_log.AddLog("Model loded from: ");
				//App->ui->my_log.AddLog(directory.c_str());
				//App->ui->my_log.AddLog("\n");
				App->modelLoader->dir = directory.c_str();
				App->modelLoader->hasChanged = true;
			}

			if (directory.substr(directory.size() - 4, directory.size()) == ".png") {
				//App->ui->my_log.AddLog("PNG dropped from: ");

				ILuint textureID;

				ilGenImages(1, &textureID);
				ilBindImage(textureID);
				ilLoadImage(directory.c_str());

				glGenTextures(1, &textureID);

				int width, height, nrComponents;

				width = ilGetInteger(IL_IMAGE_WIDTH);
				height = ilGetInteger(IL_IMAGE_HEIGHT);

				GLuint texture = ilutGLBindTexImage();

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				ILenum Error;
				Error = ilGetError();

				//std::cout << Error << std::endl;

				ILubyte* data = ilGetData();

				if (data)
				{
					glBindTexture(GL_TEXTURE_2D, textureID);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					glBindTexture(GL_TEXTURE_2D, 0);
					ilDeleteImages(1, &textureID);

					std::string _path = directory.substr(directory.find_last_of('\\') + 1);
					directory = directory.substr(0, directory.find_last_of('\\'));
					App->ui->my_log.AddLog(_path.c_str());
					App->modelLoader->model.TextureFromFile(_path, directory);

					App->modelLoader->model.textures_loaded.clear();
					Texture texture;
					texture.id = App->modelLoader->model.TextureFromFile(_path, directory);
					texture.type = "texture_diffuse";
					texture.path = _path;
					App->modelLoader->model.textures_loaded.push_back(texture);

				}
				else
				{
					std::cout << "Texture failed to load at path: " << directory << std::endl;
					ilDeleteImages(1, &textureID);
				}
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
