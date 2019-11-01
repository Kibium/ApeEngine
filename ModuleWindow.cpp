#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleUI.h"
#include <iostream>

using namespace std;

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	//LOG("Init SDL window & surface");
	App->ui->my_log.AddLog("Init SDL window & surface \n");

	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

		if (fullscreen)
			flags |= SDL_WINDOW_FULLSCREEN;
		
		if (!bordered)
			flags |= SDL_WINDOW_BORDERLESS;


		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface

			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}


update_status ModuleWindow::Update() {

	if (fullscreen) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		fulldesktop = false;

	}
	else
		SDL_SetWindowFullscreen(window, SDL_FALSE);	

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
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

