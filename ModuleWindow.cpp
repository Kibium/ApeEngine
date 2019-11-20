#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <SDL_image.h>
#include "ModuleUI.h"

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
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL;

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
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
			SDL_Surface* surface = IMG_Load("../crown.png");
			if (surface == nullptr)
				App->ui->my_log.AddLog("Window image not found \n");
			SDL_SetWindowIcon(window, surface);
			//SDL_SetWindowIcon(window, icon);

		}
	}

	return ret;
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

