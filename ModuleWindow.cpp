#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <SDL_image.h>
#include "ModuleUI.h"
#include <GL/glew.h>

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}


float ModuleWindow::GetWidth() {

	SDL_GetWindowSize(window, &width, &height);
	return width;
}
float ModuleWindow::GetHeight() {

	SDL_GetWindowSize(window, &width, &height);
	return height;
}

// Called before render is available
bool ModuleWindow::Init()
{
	App->ui->my_log.AddLog("Init SDL window, surface and icon\n");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{

			//Set window icon
			screen_surface = SDL_GetWindowSurface(window);
			SDL_Surface* surface = IMG_Load("../crown.png");


			if (surface == nullptr)
				App->ui->my_log.AddLog("Window image not found \n");

			SDL_SetWindowIcon(window, surface);

		}
	}

	return ret;
}

update_status ModuleWindow::Update() {
	
	if (fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		glViewport(0, 0, GetWidth(), GetHeight());
		fulldesktop = false;

	}
	else {
		SDL_SetWindowFullscreen(window, SDL_FALSE);
		glViewport(0, 0, GetWidth(), GetHeight());
	}
		

	if (bordered)
		SDL_SetWindowBordered(window, SDL_TRUE);
	else
		SDL_SetWindowBordered(window, SDL_FALSE);


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

